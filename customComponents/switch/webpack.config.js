const webpack = require('webpack')

module.exports = {
  entry: __dirname + '/index.js',
  output: {
    path: __dirname + '/dist',
    filename: 'index.js'
  },
  module: {
    rules: [{
      test: /\.scss$/,
      use: [{
          loader: "style-loader" // 将 JS 字符串生成为 style 节点
      }, {
          loader: "css-loader" // 将 CSS 转化成 CommonJS 模块
      }, {
          loader: "sass-loader" // 将 Sass 编译成 CSS
      }]
    }]
  }
}
