import type { Message } from '~/types/common'

export default (message: Message) => {
  const progress = ref(0)

  watchEffect(() => {
    const { startTimeStamp = 0, endTimeStamp = 0 } = message
    try {
      progress.value = Number((startTimeStamp / endTimeStamp).toFixed(2))

      if (progress.value > 1)
        progress.value = 1
    }
    catch (e) {
      progress.value = 0
    }
  })

  return progress
}
