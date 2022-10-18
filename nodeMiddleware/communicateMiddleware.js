const ws = require('ws');
const { error } = require('./log');

function startNetToMiddlewareProxy() {
  const server = new ws.Server({
    port: 13430,
  });

  server.on('connection', (socket, req) => {
    global.toNetSocket = socket;
    socket.on('message', (data) => {
      try {
        const msg = data.toString()
        global.toFrontSocket?.send(msg)
        global.netToMiddlewareMsgCB?.forEach(item => item(data))
      } catch (e) {
        error(`e: ${e}, 歌词中间件服务器13430`)
        console.error(e)
      }
    });

    socket.on('close', () => {
      global.toFrontSocket?.send(JSON.stringify({ status: 'stop' }))
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
    global.toFrontSocket = socket

    socket.on('message', (data) => {
      if (data.toString() === 'init') {
        global.toNetSocket?.send('init')
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

exports.startLrcCommunicateMiddleware = () => {
  startMiddlewareToFront()
  startNetToMiddlewareProxy()
}
