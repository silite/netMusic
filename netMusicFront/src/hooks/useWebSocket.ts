import ReconnectReconnectingWebSocket from 'reconnecting-websocket'
import type { Message, WsKey } from '~/types/common'

export default () => {
  const status = ref('loading')
  const message = reactive<Message>({})

  const rws = new ReconnectReconnectingWebSocket('ws://localhost:12449')

  rws.addEventListener('open', () => {
    status.value = 'connected'
  })

  rws.addEventListener('message', (evt) => {
    const data = evt.data as string

    status.value = data.includes('error') ? 'error' : 'success'

    const [key, value] = data.split('*ss*')
    message[key as WsKey] = value
  })

  rws.addEventListener('close', () => {
    status.value = 'loading'
  })

  return {
    status,
    message,
  }
}
