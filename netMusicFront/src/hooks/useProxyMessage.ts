import useFetch from './useFetch'
import type { Message } from '~/types/common'

export default async (message: Message) => {
  const proxyMessage = reactive(message)

  watchEffect(() => {
    proxyMessage.subSongName = proxyMessage.subSongName?.split('(')?.join('')?.split(')')?.join('')
    proxyMessage.songName = proxyMessage.songName?.trim()
  })

  watchEffect(async () => {
    const { lyric, tlyric } = await useFetch<{ id?: string }, { lyric: string; tlyric: string }>('lyric', { id: proxyMessage.songID })
    proxyMessage.lyric = lyric
    proxyMessage.tlyric = tlyric
  })

  return { proxyMessage }
}
