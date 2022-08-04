import type { Message } from '~/types/common'
import { str2Time } from '~/utils/common'

export default (message: Message) => {
  const progress = ref(0)

  watchEffect(() => {
    const { playTime, totalTime } = message
    try {
      progress.value = Number((str2Time(playTime) / str2Time(totalTime)).toFixed(2))

      if (progress.value > 1)
        progress.value = 1
    }
    catch (e) {
      progress.value = 0
    }
  })

  return progress
}
