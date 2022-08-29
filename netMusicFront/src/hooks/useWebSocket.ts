import ReconnectingWebSocket from 'reconnecting-websocket'
import type { Message, WsKey } from '~/types/common'

export default () => {
  const message = reactive<Message>({})

  const rws = new ReconnectingWebSocket('ws://localhost:12449')

  rws.addEventListener('open', () => {
    rws.send('init')
  })

  rws.addEventListener('message', (evt) => {
    const data = JSON.parse(evt.data as string)

    const key = Object.keys(data)[0] as WsKey
    const value = Object.values(data)[0] as string
    if (message[key] !== value)
      message[key] = value
  })

  rws.addEventListener('close', () => {
    message.status = 'stop'
  })

  return {
    message,
  }
}
