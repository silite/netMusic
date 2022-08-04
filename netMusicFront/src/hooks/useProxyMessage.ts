import useFetch from './useFetch'
import type { Message } from '~/types/common'

export default async (message: Message) => {
  const proxyMessage = reactive(message)

  watchEffect(async () => {
    const { lyric } = await useFetch<{ id?: string }, { lyric: string }>('lyric', { id: proxyMessage.id })
    proxyMessage.lyric = lyric
  })

  return { proxyMessage }
}
