const fs = require('fs')
const userInfo = fs.readFileSync('./userInfo.json', 'utf-8')
const { roomId, cookie, csrf } = JSON.parse(userInfo)

const params = {
  bubble: '1',
  color: '16777215',
  mode: '1',
  fontsize: '25',
  csrf,
  csrf_token: csrf
}

const sendAPi = 'https://api.live.bilibili.com/msg/send'

module.exports = {
  roomId,
  cookie,
  params,
  sendAPi
}
