require('./index.scss')

window.createSwitch = function createSwitch() {
  const container = document.createElement('label')
  container.className = 'switch'
  const checkbox = document.createElement('input')
  checkbox.type = 'checkbox'
  const div = document.createElement('div')
  div.appendChild(document.createElement('span'))
  container.appendChild(checkbox)
  container.appendChild(div)
  return container
}
