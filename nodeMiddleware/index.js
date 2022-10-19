const { startLrcServiceAndFrontendStatic } = require('./lyricAndStatic')
const { startLrcCommunicateMiddleware } = require('./communicateMiddleware')
const { start } = require('./orderSong')
const { error } = require('./log')
const { login } = require('./dannmuku')


try {
  // 查询歌词 和 歌词姬前端打包页托管
  startLrcServiceAndFrontendStatic()
  // 网易云和歌词姬通信中间件
  startLrcCommunicateMiddleware()

  login()

  global.dev = true
  start()
} catch (e) {
  error(`func: main, e: ${e}`)
}
