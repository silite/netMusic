const WS = require('ws')
const ReconnectingWebSocket = require('reconnecting-websocket')
const pako = require('pako')
const http = require('https')
const axios = require('axios')
const FormData = require('form-data')
const {sendAPi, getCookies, params, roomId, getLogInfo} = require('./loginConstant')
const {error, log} = require('./log')
const QRCode = require('qrcode')
const fs = require('fs')
const {writeWholeFile} = require("./localFileDatabase");
const notifier = require('node-notifier');

const textEncoder = new TextEncoder('utf-8')
const textDecoder = new TextDecoder('utf-8')

const readInt = function (buffer, start, len) {
  let result = 0
  for (let i = len - 1; i >= 0; i--) {
    result += Math.pow(256, len - i - 1) * buffer[start + i]
  }
  return result
}

const writeInt = function (buffer, start, len, value) {
  let i = 0
  while (i < len) {
    buffer[start + i] = value / Math.pow(256, len - i - 1)
    i++
  }
}

const encode = function (str, op) {
  let data = textEncoder.encode(str)
  let packetLen = 16 + data.byteLength
  let header = [0, 0, 0, 0, 0, 16, 0, 1, 0, 0, 0, op, 0, 0, 0, 1]
  writeInt(header, 0, 4, packetLen)
  return new Uint8Array(header.concat(...data)).buffer
}
const decode = function (blob) {
  return new Promise(function (resolve, reject) {
    let buffer = new Uint8Array(blob)
    let result = {}
    result.packetLen = readInt(buffer, 0, 4)
    result.headerLen = readInt(buffer, 4, 2)
    result.ver = readInt(buffer, 6, 2)
    result.op = readInt(buffer, 8, 4)
    result.seq = readInt(buffer, 12, 4)
    if (result.op === 5) {
      result.body = []
      if (result.ver === 0) {
        let data = buffer.slice(result.headerLen, result.packetLen)
        let body = textDecoder.decode(data)
        result.body.push(JSON.parse(body))
      } else if (result.ver === 2) {
        let newbuffer = pako.inflate(
            buffer.slice(result.headerLen, result.packetLen)
        )
        let offset = 0
        while (offset < newbuffer.length) {
          let packetLen = readInt(newbuffer, offset + 0, 4)
          let headerLen = 16 // readInt(buffer,offset + 4,4)
          let data = newbuffer.slice(offset + headerLen, offset + packetLen)
          /**
           *    引入pako做message解压处理，具体代码链接如下
           *    https://github.com/nodeca/pako/blob/master/dist/pako.js
           */
          let body = textDecoder.decode(data)
          if (body) {
            result.body.push(JSON.parse(body))
          }
          offset += packetLen
        }
      }
    } else if (result.op === 3) {
      result.body = {
        count: readInt(buffer, 16, 4),
      }
    }
    resolve(result)
  })
}

function connecting(room, msgHandler) {
  const ws = new ReconnectingWebSocket(
      'wss://broadcastlv.chat.bilibili.com:2245/sub',
      [],
      {
        WebSocket: WS,
      }
  )
  ws.onopen = function () {
    ws.send(
        encode(
            JSON.stringify({
              roomid: Number(room),
            }),
            7
        )
    )
  }

  let heartBeatTask = setInterval(function () {
    ws.send(encode('', 2))
  }, 30000)

  ws.onmessage = async function (msgEvent) {
    const packet = await decode(msgEvent.data)
    switch (packet.op) {
      case 8:
        break
      case 3:
        msgHandler(3, packet.body.count)
        break
      case 5:
        packet.body.forEach((body) => {
          msgHandler(5, getDanmuMsg(body || {}), body)
        })
        break
      default:
        console.warn(packet)
    }
  }

  return function () {
    clearInterval(heartBeatTask)
    ws.close()
  }
}

const cutUserName = (userName) => {
  if (userName.length > 10) return `${userName.slice(0, 10)}...`
  return userName
}

function getDanmuMsg({cmd, info} = {}) {
  if (cmd === 'DANMU_MSG') {
    return {
      msg: info[1],
      userInfo: {
        uid: info[2][0],
        userName: info[2][1],
        shortName: cutUserName(info[2][1] || ''),
      },
      fanBrand: {
        level: info[3][0],
        brandName: info[3][1],
        brandNum: info[3][3],
      },
    }
  }
}

function checkLiveStatus(room) {
  return new Promise(function (resolve, reject) {
    http
        .get(
            `https://api.live.bilibili.com/room/v1/Room/room_init?id=${room}`,
            (res) => {
              let data = ''
              res.on('data', (chunk) => {
                data += chunk
              })
              res.on('end', () => {
                try {
                  const result = JSON.parse(data)
                  if (result.code === 0) {
                    resolve(!!result.data.live_status)
                  } else {
                    resolve(false)
                  }
                } catch (e) {
                  error(`解析直播间状态 ,e: ${e}`)
                }
              })
            }
        )
        .on('error', (e) => {
          error(`直播状态接口调用, e${e}`)
          console.error(e)
        })
  })
}

let interval
const clearLoop = () => {
  if (interval) {
    clearInterval(interval)
    interval = undefined
  }
}

async function createLoginLoop(oauthKey) {
  const getUrlParams = (url) => {
    const purl = new URL(url)
    const paramsStr = purl.search.slice(1)
    const params = new URLSearchParams(paramsStr)
    return {
      DedeUserID: Number.parseInt(params.get('DedeUserID') || ''),
      DedeUserID__ckMd5: params.get('DedeUserID__ckMd5') || '',
      SESSDATA: params.get('SESSDATA') || '',
      bili_jct: params.get('bili_jct') || '',
      Expires: Number.parseInt(params.get('Expires') || ''),
      sid: params.get('sid') || '',
    }
  }

  return new Promise((resolve) => {
    interval = setInterval(async () => {
      const response = await axios.post('http://passport.bilibili.com/qrcode/getLoginInfo', { oauthKey }, {
        headers: {
          'Content-Type': 'application/x-www-form-urlencoded',
        },
      })

      const {data: loginResponse, headers} = response

      if (typeof loginResponse.data === 'object') {
        clearLoop()
        const params = getUrlParams(loginResponse.data.url)
        const { data: {data} } = await axios.get(`http://api.bilibili.com/x/web-interface/card?mid=${params.DedeUserID}`)
        const userInfo = {
          oauthKey,
          mid: params.DedeUserID,
          midmd5: params.DedeUserID__ckMd5,
          bili_jct: params.bili_jct,
          expires: params.Expires,
          sessdata: params.SESSDATA,
          sid: params.sid === '' ? headers['set-cookie'][0].split(';')[0].split('=')[1] : params.sid,
          mname: data.card.name,
          avatarUrl: data.card.face,
          lastLogin: new Date().getTime(),
        }
        writeWholeFile('./logInfo', JSON.stringify(userInfo))
        resolve(userInfo)
      }
    }, 3000)
  })
}

function delQRCode() {
  try {
    fs.unlink('./login.png', (err) => {
      error(`删除login二维码, e: ${err}`)
    })
  } catch (e) {
    error(`删除login二维码, e: ${e}`)
  }
}

async function login() {
  const {lastLogin, expires, mname} = getLogInfo()
  if (lastLogin && lastLogin + expires * 1000 >= new Date().getTime()) {
    notifier.notify({
      title: mname,
      message: `登录成功`
    })
    return
  }

  notifier.notify({
    title: '嗨嗨嗨',
    message: '需要扫描程序文件夹下的login.png二维码登录，否则不能发送弹幕'
  })

  const {data: {data}} = await axios.get('http://passport.bilibili.com/qrcode/getLoginUrl')
  const {url, oauthKey} = data
  delQRCode()
  const qrcodeImgUrl = await QRCode.toDataURL(url)
  const qrcodeImgBuffer = new Buffer(qrcodeImgUrl.replace(/^data:image\/\w+;base64,/,""), 'base64')

  fs.writeFile('./login.png', qrcodeImgBuffer, function (err) {
    log('生成登录二维码')
    if (err) {
    } else {
    }
  })

  const userInfo = await createLoginLoop(oauthKey)

  if (userInfo) {
    notifier.notify({
      title: userInfo.mname,
      message: `登录成功`
    })
  }
}

async function sendDanmu(msg, roomid = roomId) {
  const form = new FormData()
  for (let [key, value] of Object.entries({...params, msg, roomid, rnd: new Date().valueOf()})) {
    form.append(key, value)
  }
  try {
    const res = await axios.post(sendAPi, form, {
      headers: {
        cookie: getCookies(),
      },
    })
    if (res.status === 200) {
      log(`发送弹幕: ${msg}`)
    } else {
      error(`弹幕发送失败, res: ${res}`)
    }
  } catch (e) {
    error(`e: ${e}, fun: sendDanmu`)
  }
}

module.exports = {
  connecting,
  checkLiveStatus,
  sendDanmu,
  login,
}
