import Lyric from 'lrc-file-parser'
import type { Message } from '~/types/common'
import { str2Time } from '~/utils/common'
type lines = Parameters<ConstructorParameters<typeof Lyric>[0]['onSetLyric']>[0]

export default (message: Message) => {
  const currentLrc = ref<{ line?: Number; text?: String; lrcLines?: lines }>({})

  const lrcParser = new Lyric({
    onPlay: (line, text) => {
      currentLrc.value = { ...currentLrc.value, line, text: `${text}*ss*${currentLrc.value?.lrcLines?.[line as number].extendedLyrics}` }
    },

    onSetLyric: (lines) => {
      currentLrc.value.lrcLines = lines
    },

    offset: -700,

    isRemoveBlankLine: true,

    lyric: '',

    translationLyric: '',

  })

  watchEffect(() => {
    lrcParser.setLyric(message.lyric || '', [message.tlyric || ''])
  })

  watchEffect(() => {
    if (message.isPlaying?.includes('True'))
      lrcParser.play(str2Time(message.playTime) * 1000)
    else lrcParser.pause()
  })

  return { currentLrc }
}
