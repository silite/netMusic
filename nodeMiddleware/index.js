const app = require('express')()
const bodyParser = require('body-parser')
const { lyric } = require('NeteaseCloudMusicApi')

app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())

app.all('*', function (req, res, next) {
  res.header('Access-Control-Allow-Origin', '*')
  res.header("Access-Control-Allow-Headers", " Origin, X-Requested-With, Content-Type, Accept");
  res.header('Access-Control-Allow-Methods', 'PUT,POST,GET,DELETE,OPTIONS')
  res.header('X-Powered-By', ' 3.2.1')
  res.header('Content-Type', 'application/json;charset=utf-8')
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
    res.send(body?.lrc)
  } catch (e) {
    res.send({ lyric: "查找失败-_-" })
  }

})

app.listen('10145', () => { })
