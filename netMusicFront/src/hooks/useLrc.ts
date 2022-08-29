import Lyric from 'lrc-file-parser'
import type { Message } from '~/types/common'
type lines = Parameters<ConstructorParameters<typeof Lyric>[0]['onSetLyric']>[0]

export default (message: Message) => {
  const currentLrc = ref<{ line?: Number; text?: { curr?: string; next?: string }; lrcLines?: lines }>({})

  const getLrcParser = () => {
    return new Lyric({
      onPlay: (line) => {
        const lineNum = line as number
        const getLrc = (num: number) => {
          const currentLrcObj = currentLrc.value?.lrcLines?.[num]
          return `${currentLrcObj?.text || ''}*miko*${currentLrcObj?.extendedLyrics || ''}`
        }
        currentLrc.value = { ...currentLrc.value, line, text: { curr: getLrc(lineNum), next: getLrc(lineNum + 1) } }
      },

      onSetLyric: (lines) => {
        currentLrc.value.lrcLines = lines
      },

      offset: 0,

      isRemoveBlankLine: true,

      lyric: '',

      translationLyric: '',

    })
  }

  const lrcParser = ref<any>(getLrcParser())

  watch(() => message?.songID, () => {
    currentLrc.value = {}
    lrcParser.value = getLrcParser()
  })

  watchEffect(() => {
    lrcParser.value.setLyric(message.lyric || '', [message.tlyric || ''])
  })

  watchEffect(() => {
    if (message.isPlaying === true)
      lrcParser.value.play((message.startTimeStamp || 0) * 1000)
    else lrcParser.value.pause()
  })

  const getLrcItem = (str?: string) => str?.split('*miko*').filter(item => Boolean(item)) || ''
  const lrc = computed(() => [getLrcItem(currentLrc.value.text?.curr), getLrcItem(currentLrc.value.text?.next)].filter(item => item?.length))

  return lrc
}
