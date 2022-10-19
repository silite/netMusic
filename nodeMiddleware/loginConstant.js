const fs = require('fs')
const config = fs.readFileSync('./config.json', 'utf-8')

const { roomId } = JSON.parse(config)

function getCookies() {
  const { bili_jct, mid, sid, midmd5, sessdata } = getLogInfo()
  if (!mid)
    return ''
  let cookies = ''
  cookies += `sid=${sid}; `
  cookies += `DedeUserID=${mid}; `
  cookies += `DedeUserID__ckMd5=${midmd5}; `
  cookies += `SESSDATA=${sessdata}; `
  cookies += `bili_jct=${bili_jct}`
  return cookies
}

function getLogInfo () {
  return JSON.parse(fs.readFileSync('./logInfo', 'utf-8') || "{}")
}

const params = {
  bubble: '1',
  color: '16777215',
  mode: '1',
  fontsize: '25',
  csrf: getLogInfo().bili_jct,
  csrf_token: getLogInfo().bili_jct
}

const sendAPi = 'https://api.live.bilibili.com/msg/send'

module.exports = {
  roomId,
  getCookies,
  getLogInfo,
  params,
  sendAPi
}
