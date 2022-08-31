<script setup lang="ts">
import { Circle } from 'progressbar.js'
interface propType {
  songName?: string
  player?: string
  progress?: number
  isPlaying?: boolean
  subSongName?: string
}
const props = defineProps<propType>()

const infoRef = ref()
const infoHeight = ref(0)
watch(() => [props.songName, props.subSongName, props.player], () => {
  nextTick(() => {
    infoHeight.value = infoRef.value?.offsetHeight
  })
}, { immediate: true })

const getPositiveNum = (num: number) => {
  return num < 0 ? 0 : num
}
const progressContentStyle = computed(() => ({ height: `${infoHeight.value}px` }))
const imgStyle = computed(() => {
  const val = getPositiveNum((infoHeight.value || 0) - 20)
  return { width: `${val}px`, height: `${val}px` }
})
const progressStyle = computed(() => {
  const val = (infoHeight.value || 0) - 10
  return { width: `${val}px`, height: `${val}px` }
})

const progressInstance = ref()
onMounted(() => {
  progressInstance.value = new Circle(document.querySelector('#progress'), {
    strokeWidth: 6,
    easing: 'easeInOut',
    duration: 1000,
    color: '#ffecc0',
    trailColor: 'transparent',
    trailWidth: 1,
    svgStyle: null,
  })
})

watch(() => props.progress, () => {
  progressInstance.value?.animate(props.progress || 0)
})
</script>

<template>
  <div
    flex="~"
    max-w-400px
  >
    <div
      :style="progressContentStyle"
      flex
      items-center
      pr-4
    >
      <img src="/logo.png" :style="imgStyle" rounded-full absolute left-5px>
      <div id="progress" :style="progressStyle" />
    </div>
    <div ref="infoRef" flex="~ col" items-end justify-center>
      <div
        flex="~"
        transition="all-300"
        :style="{ animation: '15s wordsLoops 3s infinite' }"
      >
        <span
          text-xl
          whitespace-nowrap
          pl-1
          font="bold tianshi-blue"
          :style="{
            textShadow: 'rgb(137 166 233) -1px 1px 0px, rgb(169 82 118 / 55%) 1px 1px 0px',
          }"
        >
          {{ songName }}
        </span>
        <span
          text-xl
          whitespace-nowrap
          pl-1
          font="bold tianshi-blue"
          :style="{
            textShadow: 'rgb(137 166 233) -1px 1px 0px, rgb(169 82 118 / 55%) 1px 1px 0px',
          }"
        >
          {{ songName }}
        </span>
      </div>

      <span
        text-xs
        color-violet-400
        whitespace-nowrap
        overflow-hidden
        text-right
        pt-1
        style="text-shadow: -1px 1px 0 #89a6e9, 1px 1px 0 #fb86b78c;"
      >
        {{ subSongName }}
      </span>
      <span
        text-lg
        font-tianshi-pink
        style="text-shadow: -1px 1px 0 #abc4fd, 1px 1px 0 #b7c5df8c;"
      >{{ player }}</span>
    </div>
  </div>
</template>

<style>
@keyframes wordsLoops {
    0% {
        transform: translate3d(0, 0, 0);
    }

    100% {
        transform: translate3d(-50%, 0, 0);
    }
}
</style>
