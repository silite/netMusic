const fs = require('fs')

function writeLog(type, msg) {
  const date = new Date()
  const dir = date.toLocaleDateString().split('/').join('-')
  try {
    fs.mkdir(`./${dir}`, () => {})
    fs.writeFileSync(`./${dir}/log.miko`, `[${type}] ${date.toLocaleString()} ${msg}` + '\n', {
      flag: 'a+'
    })
  } catch (e) {

  }
}

function log(msg) {
  writeLog('log', msg)
}

function error(msg) {
  writeLog('error', msg)
}

module.exports = {
  log,
  error,
}
