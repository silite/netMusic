import useFetch from './useFetch'
import type { Message } from '~/types/common'
import { str2Time, time2Str } from '~/utils/common'

export default async (message: Message) => {
  const proxyMessage = reactive(message)

  watchEffect(() => {
    if (proxyMessage.isPlaying && proxyMessage.playTime)
      proxyMessage.playTime = time2Str(str2Time(proxyMessage.playTime) + 1)
  })

  watchEffect(async () => {
    const { lyric, tlyric } = await useFetch<{ id?: string }, { lyric: string; tlyric: string }>('lyric', { id: proxyMessage.id })
    proxyMessage.lyric = lyric
    proxyMessage.tlyric = tlyric
  })

  return { proxyMessage }
}
