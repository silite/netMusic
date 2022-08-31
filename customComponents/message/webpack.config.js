module.exports = {
  entry: __dirname + '/index.js',
  output: {
    path: __dirname + '/dist',
    filename: 'index.js'
  },
  module: {
    rules: [{
      test: /\.css$/,
      use: [{
          loader: "style-loader" // 将 JS 字符串生成为 style 节点
      }, {
          loader: "css-loader" // 将 CSS 转化成 CommonJS 模块
      }]
    }]
  }
}
