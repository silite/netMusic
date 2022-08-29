const express = require('express')
const bodyParser = require('body-parser')
const { lyric } = require('NeteaseCloudMusicApi')
const path = require('path')
const ws = require('ws')
const ReconnectReconnectingWebSocket = require('reconnecting-websocket')

const app = express()

app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())

app.all('*', function (req, res, next) {
  res.header('Access-Control-Allow-Origin', '*')
  res.header("Access-Control-Allow-Headers", " Origin, X-Requested-With, Content-Type, Accept");
  res.header('Access-Control-Allow-Methods', 'PUT,POST,GET,DELETE,OPTIONS')
  res.header('X-Powered-By', ' 3.2.1')
  next()
})

app.post('/lyric', async (req, res) => {
  try {
    const { id } = req.body
    if (!id) {
      res.send({})
      return
    }

    const { body } = await lyric({ id })
    res.send({ lyric: body?.lrc?.lyric, tlyric: body?.tlyric?.lyric })
  } catch (e) {
    res.send({ lyric: "查找失败-_-" })
  }
})

app.use('/', express.static(path.join(__dirname, 'dist')))

app.listen('10145', () => { })


let toFrontSocket;
let toNetSocket;
function startNetToMiddlewareProxy() {
  const server = new ws.Server({
    port: 13430,
  });

  server.on('connection', (socket, req) => {
    toNetSocket = socket;
    socket.on('message', (data) => {
      try {
        const msg = data.toString()
        toFrontSocket.send(msg)
      } catch (e) {
        console.error(e)
      }
    });

    socket.on('close', () => {
      toFrontSocket.send(JSON.stringify({ status: 'stop' }))
    })

    socket.on('error', (error) => {
      startNetToMiddlewareProxy()
    })
  })
}

function startMiddlewareToFront() {
  const websocketProxyServer = new ws.Server({
    port: 12449,
  });

  websocketProxyServer.on('connection', (socket, req) => {
    toFrontSocket = socket

    socket.on('message', (data) => {
      if (data.toString() === 'init') {
        toNetSocket.send('init')
      }
    });

    // 监听的是，服务端收到了客户端关闭连接的事件，由客户端发起的关闭
    socket.on('close', (code, reason) => {
    });

    // 监听的是，WebSocket 通信过程中出错的事件
    socket.on('error', (error) => {
      startMiddlewareToFront()
    });
  });
}

startMiddlewareToFront()
startNetToMiddlewareProxy()
