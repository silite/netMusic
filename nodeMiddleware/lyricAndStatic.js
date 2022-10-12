const express = require('express')
const bodyParser = require('body-parser')
const { lyric } = require('NeteaseCloudMusicApi')
const path = require('path')
const { error } = require('./log')

function startLrcServiceAndFrontendStatic() {
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
      error(`歌词查找接口调用失败, e: ${e}`)
      res.send({ lyric: "查找失败-_-" })
    }
  })

  app.use('/', express.static(path.join(__dirname, 'dist')))

  app.listen('10145', () => { })
}
exports.startLrcServiceAndFrontendStatic = startLrcServiceAndFrontendStatic
