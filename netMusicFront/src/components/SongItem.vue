<script setup lang="ts">
import { Circle } from 'progressbar.js'
import anime from 'animejs'
import { getDomWidth } from '~/utils/common'

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
const infoWidth = ref(Infinity)
onMounted(() => {
  const font = new FontFace('zpix', 'url(zpix.woff2)')
  font.load().then(() => {
    watch(() => [props.songName, props.subSongName, props.player], () => {
      nextTick(() => {
        infoHeight.value = infoRef.value?.offsetHeight
        infoWidth.value = infoRef.value?.offsetWidth
      })
    }, { immediate: true })
  })
})
const progressContentStyle = computed(() => ({ height: `${infoHeight.value}px` }))

const getPositiveNum = (num: number) => {
  return num < 0 ? 0 : num
}
const imgStyle = computed(() => {
  const val = getPositiveNum((infoHeight.value || 0) - 20)
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

const songNameWidth = ref<number>(0)
watchEffect(async () => {
  songNameWidth.value = await getDomWidth(props.songName || '', { fontSize: '1.25rem', lineHeight: '1.75rem' } as CSSStyleDeclaration) + 5
})
const subSongNameWidth = ref<number>(0)
watchEffect(async () => {
  subSongNameWidth.value = await getDomWidth(props.subSongName || '', { fontSize: '0.875rem', lineHeight: '1.25rem' } as CSSStyleDeclaration) + 5
})
const playerWidth = ref<number>(0)
watchEffect(async () => {
  playerWidth.value = await getDomWidth(props.player || '', { fontSize: '1.125rem', lineHeight: '1.75rem' } as CSSStyleDeclaration) + 5
})

const maxWidthInfo = computed(() => {
  const resList = [{ songName: songNameWidth.value }, { subSongName: subSongNameWidth.value }, { player: playerWidth.value }]
  let maxValue = 0
  let maxKey
  resList.forEach((item) => {
    const [key, value] = Object.entries(item)[0]
    if (maxValue < value) {
      maxValue = value
      maxKey = key
    }
  })
  return { key: maxKey, value: maxValue }
})

const playLoopAnime = computed(() => (key: string) => maxWidthInfo.value.key === key && infoWidth.value + 5 < maxWidthInfo.value.value)
const transformValue = computed(() => `-${maxWidthInfo.value.value}px`)
const playTime = computed(() => Math.max(props.songName?.length || 0, props.subSongName?.length || 0, props.player?.length || 0))
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
      <img src="/logo.png" rounded-full absolute left-5px :style="imgStyle">
      <div id="progress" />
    </div>
    <div ref="infoRef" flex="~ col" justify-center overflow-hidden>
      <div
        id="songName"
        flex="~"
        :style="{
          animation: playLoopAnime('songName') ? `${playTime}s wordsLoops 3s infinite` : '',
          justifyContent: maxWidthInfo.key !== 'songName' ? 'flex-end' : '',
        }"
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
          v-if="playLoopAnime('songName')"
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

      <div
        id="subSongName"
        flex="~"
        :style="{
          animation: playLoopAnime('subSongName') ? `${playTime}s wordsLoops 3s infinite` : '',
          justifyContent: maxWidthInfo.key !== 'subSongName' ? 'flex-end' : '',
        }"
      >
        <div
          text-sm
          color-violet-400
          whitespace-nowrap
          overflow-hidden
          pt-1
          pl-1
          :style="{
            textShadow: '-1px 1px 0 #89a6e9, 1px 1px 0 #fb86b78c',
          }"
          style="text-shadow: -1px 1px 0 #89a6e9, 1px 1px 0 #fb86b78c;"
        >
          {{ subSongName }}
        </div>
        <span
          v-if="playLoopAnime('subSongName')"
          text-sm
          color-violet-400
          whitespace-nowrap
          overflow-hidden
          pt-1
          pl-1
          style="text-shadow: -1px 1px 0 #89a6e9, 1px 1px 0 #fb86b78c;"
        >
          {{ subSongName }}
        </span>
      </div>

      <div
        id="player"
        flex="~"
        :style="{
          animation: playLoopAnime('player') ? `${playTime}s wordsLoops 3s infinite` : '',
          justifyContent: maxWidthInfo.key !== 'player' ? 'flex-end' : '',
        }"
      >
        <span
          text-lg
          pl-1
          whitespace-nowrap
          font-tianshi-pink
          text-right
          :style="{
            textShadow: '-1px 1px 0 #abc4fd, 1px 1px 0 #b7c5df8c',
          }"
        >{{ player }}</span>
        <span
          v-if="playLoopAnime('player')"
          text-lg
          pl-1
          whitespace-nowrap
          font-tianshi-pink
          text-right
          style="text-shadow: -1px 1px 0 #abc4fd, 1px 1px 0 #b7c5df8c;"
        >{{ player }}</span>
      </div>
    </div>
  </div>
</template>

<style>
@keyframes wordsLoops {
    0% {
        transform: translate3d(0, 0, 0);
    }

    100% {
        transform: translate3d(v-bind(transformValue), 0, 0);
    }
}
</style>
