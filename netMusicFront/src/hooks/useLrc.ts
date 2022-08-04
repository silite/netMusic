import Lyric from 'lrc-file-parser'
import type { Message } from '~/types/common'
import { str2Time } from '~/utils/common'
type lines = Parameters<ConstructorParameters<typeof Lyric>[0]['onSetLyric']>[0]

export default (message: Message) => {
  const currentLrc = ref<{ line?: Number; text?: { curr?: string; next?: string }; lrcLines?: lines }>({})

  const lrcParser = new Lyric({
    onPlay: (line) => {
      const lineNum = line as number
      const getLrc = (num: number) => {
        const currentLrcObj = currentLrc.value?.lrcLines?.[num]
        return `${currentLrcObj?.text || ''}*ss*${currentLrcObj?.extendedLyrics || ''}`
      }
      currentLrc.value = { ...currentLrc.value, line, text: { curr: getLrc(lineNum), next: getLrc(lineNum + 1) } }
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
    // eslint-disable-next-line @typescript-eslint/ban-ts-comment
    // @ts-expect-error
    lrcParser.setLyric(message.lyric || '', [message.tlyric || ''])
  })

  watchEffect(() => {
    if (message.isPlaying?.includes('True'))
      lrcParser.play(str2Time(message.playTime) * 1000)
    else lrcParser.pause()
  })

  const getLrcItem = (str?: string) => str?.split('*ss*').filter(item => Boolean(item)) || ''
  const lrc = computed(() => [getLrcItem(currentLrc.value.text?.curr), getLrcItem(currentLrc.value.text?.next)].filter(item => item?.length))

  return lrc
}
