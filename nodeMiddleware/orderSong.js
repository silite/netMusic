const { cloudsearch } = require('NeteaseCloudMusicApi')
const { checkLiveStatus, connecting, sendDanmu } = require('./dannmuku')
const { readFile, writeWholeFile } = require('./localFileDatabase')
const { log, error } = require('./log')
const { roomId } = require('./loginConstant')

const databaseName = 'orderSong.miko'

if (!global.netToMiddlewareMsgCB) global.netToMiddlewareMsgCB = []
global.netToMiddlewareMsgCB.push((data) => {
  try {
    const { type, songId } = JSON.parse(data.toString())
    const currOrderList = readFile(databaseName)

    if (type === 'init') sendPlayList(currOrderList)
    else if (type !== 'pop') return

    const targetPopIndex = currOrderList.findIndex(({ id }) => +songId === id)
    if (targetPopIndex === -1) return

    const sliceRes = JSON.stringify(currOrderList.slice(0, targetPopIndex).concat(currOrderList.slice(targetPopIndex + 1)))
    log(`切歌, songId: ${songId}`)
    writeWholeFile(databaseName, sliceRes, (err) => {
      error(`func: net to writeWholeFile, e: ${err}`)
    })
    sendPlayList(sliceRes)
  } catch (e) {
    error(`func: netToMiddlewareMsgCB, e: ${e}`)
  }
})

function sendPlayList(orderList) {
  global.toNetSocket?.send(JSON.stringify({ type: 'play', orderList }))
}

function getOrderSongName(msg) {
  if (!/^点歌/.test(msg)) return false
  const songName = (msg.split('点歌')[1] || '').trim()
  return songName || false
}

function cancelSong({ msg, shortName, uid }) {
  const currOrderList = readFile(databaseName)

  if (/^取消点歌$/.test(msg)) {
    const reverseOrderSongIndex = [...currOrderList]
      .reverse()
      .findIndex(({ uid: listUid }) => uid === listUid)
    if (reverseOrderSongIndex === -1) {
      sendDanmu(`@${shortName} 还没有点歌哦`)
    } else {
      const targetIndex = currOrderList.length - reverseOrderSongIndex - 1
      const orderList = JSON.stringify(
        currOrderList
          .slice(0, targetIndex)
          .concat(currOrderList.slice(targetIndex + 1))
      )
      writeWholeFile(
        databaseName,
        orderList
      )
      sendPlayList(orderList)
      sendDanmu(`@${shortName} 已取消最新一首点歌`)
    }

    return true
  } else if (/^取消(所有|全部)$/.test(msg)) {
    const filterOrderList = currOrderList.filter(({ uid: listUid }) => uid === listUid)
    if (filterOrderList.length === 0) {
      sendDanmu(`${shortName} 还没有点歌哦`)
    } else {
      const orderList = JSON.stringify(
        currOrderList.filter(({ uid: listUid }) => uid !== listUid)
      )
      writeWholeFile(
        databaseName,
        orderList
      )
      sendPlayList(orderList)
      sendDanmu(`@${shortName} 已取消所有点歌`)
    }
    return true
  }
  return false
}

async function orderSong(orderInfo) {
  const { shortName, level, uid, userName, brandName, orderSongName, brandNum } =
    orderInfo

  if (brandNum !== roomId && !global.dev) {
    sendDanmu(`@${shortName} 粉丝牌可以点歌哦`)
    return
  }

  const currOrderList = readFile(databaseName)

  if (currOrderList.length >= 10) {
    sendDanmu('歌单太多首啦，忍一会吧')
    return
  }

  if (currOrderList.filter(({ uid: listUid }) => uid === listUid).length >= 3) {
    sendDanmu(`@${shortName} 点了太多首啦`)
    return
  }

  const { body } = await cloudsearch({
    keywords: orderSongName,
    type: 1,
  })
  if (body.code !== 200) {
    sendDanmu(`@${shortName} 网易云坏掉了`)
    return
  }

  const songsList = body?.result?.songs || []
  if (songsList.length === 0) {
    sendDanmu(`@${shortName} 无搜索结果`)
    return
  }

  const { id, name, ar } = songsList[0]
  const existSong = currOrderList.find(({ id: listId }) => listId === id)
  if (existSong) {
    const shortSongName = name.length > 9 ? `${name.slice(0, 9)}...` : name
    sendDanmu(`[${shortSongName}]已在列表中`)
    return
  }

  const rebuildParams = { level, uid, userName, brandName, id, name, artist: ar?.map(item => item.name).join('、') }
  const orderList = localAddSong(rebuildParams)
  sendDanmu(`@${shortName} 点歌成功`)
  log(`level: ${level}, uid: ${uid}, userName: ${userName}, songName: ${name}, songId: ${id}`)
  sendPlayList(orderList)
}

function localAddSong(orderInfo) {
  try {
    const res = JSON.stringify(readFile(databaseName).concat([orderInfo]))
    writeWholeFile(
      databaseName,
      res,
      (err) => {
        if (err) {
          console.error('error', err)
          error(`e: ${err}, fun: writeWholeFileCallBack`)
        }
      }
    )
    return res
  } catch (e) {
    error(`e: ${e}, fun: localAddSong`)
  }
}

const start = () => {
  connecting(roomId, (type, content) => {
    if (type !== 5 || !content) return
    try {
      const {
        msg,
        userInfo: { uid, userName, shortName },
        fanBrand: { level, brandName, brandNum },
      } = content

      if (cancelSong({ msg, shortName, uid })) {
        return
      }

      const orderSongName = getOrderSongName(msg)
      if (orderSongName) {
        orderSong({
          shortName,
          level,
          uid,
          userName,
          brandName,
          orderSongName,
          brandNum
        })
      }
    } catch (e) {
      error(`e: ${e}, fun: start`)
    }
  })
}

module.exports = {
  start,
}
