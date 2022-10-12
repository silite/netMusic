const { startLrcServiceAndFrontendStatic } = require('./lyricAndStatic')
const { startLrcCommunicateMiddleware } = require('./communicateMiddleware')
const { start } = require('./orderSong')

// 查询歌词 和 歌词姬前端打包页托管
// startLrcServiceAndFrontendStatic()
// 网易云和歌词姬通信中间件
// startLrcCommunicateMiddleware()

global.dev = true
start()
