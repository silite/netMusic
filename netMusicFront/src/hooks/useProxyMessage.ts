import { lyric } from 'NeteaseCloudMusicApi'
import type { Message } from '~/types/common'

export default async (message: Message) => {
  const proxyMessage = reactive(message)

  const data = await lyric({ id: message.id || '' })

  console.log(data)

  return { proxyMessage }
}
