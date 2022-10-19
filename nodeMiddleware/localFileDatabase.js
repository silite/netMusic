const fs = require('fs')
const { error } = require('./log')

function readFile(filename) {
  try {
    return JSON.parse(fs.readFileSync(filename, 'utf-8'))
  } catch (e) {
    error(`读取本地点歌文件, e: ${e}`)
    return []
  }
}

function writeWholeFile(filename, content, callback = (err) => { }) {
  try {
    fs.writeFile(filename, content, callback)
  } catch (e) {
    error(`写入${filename}, e: ${e}`)
  }
}

module.exports = {
  readFile,
  writeWholeFile,
}
