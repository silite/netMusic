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

const progressInstance = ref()
onMounted(() => {
  progressInstance.value = new Circle(document.querySelector('#progress'), {
    strokeWidth: 6,
    easing: 'easeInOut',
    duration: 1000,
    color: '#fcbbc5',
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
    items-center
    justify-center
    max-w-380px
  >
    <div
      w-60px
      h-60px
      flex
      mr-8px
      shrink-0
      justify-center
      items-center
      relative
    >
      <img src="/logo.png" rounded-full absolute left-8px w-45px h-45px>
      <div id="progress" w-55px h-55px />
    </div>
    <div ref="infoRef" flex="~ col" justify-center overflow-hidden>
      <transition>
        <div
          v-if="songName"
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
              textShadow: 'rgb(250 243 251) -1px 1px 0px, rgb(199 210 229 / 55%) 1px 1px 0px',
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
              textShadow: 'rgb(250 243 251) -1px 1px 0px, rgb(199 210 229 / 55%) 1px 1px 0px',
            }"
          >
            {{ songName }}
          </span>
        </div>
      </transition>

      <transition>
        <div
          v-if="subSongName"
          id="subSongName"
          flex="~"
          :style="{
            animation: playLoopAnime('subSongName') ? `${playTime}s wordsLoops 3s infinite` : '',
            justifyContent: maxWidthInfo.key !== 'subSongName' ? 'flex-end' : '',
          }"
        >
          <div
            text-sm
            whitespace-nowrap
            shrink-0
            pl-1
            :style="{
              textShadow: 'rgb(207 218 245) -1px 1px 0px, rgb(189 232 237 / 55%) 1px 1px 0px',
              color: 'rgb(240, 176, 219)',
            }"
          >
            {{ subSongName }}
          </div>
          <span
            v-if="playLoopAnime('subSongName')"
            text-sm
            whitespace-nowrap
            shrink-0
            pl-1
            style="text-shadow: rgb(207 218 245) -1px 1px 0px, rgb(189 232 237 / 55%) 1px 1px 0px; color: rgb(240, 176, 219);"
          >
            {{ subSongName }}
          </span>
        </div>
      </transition>

      <transition>
        <div
          v-if="player"
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
            shrink-0
            font-tianshi-pink
            text-right
            :style="{
              textShadow: 'rgb(158 176 217) -1px 1px 0px, rgb(183 197 223 / 55%) 1px 1px 0px',
            }"
          >{{ player }}</span>
          <span
            v-if="playLoopAnime('player')"
            text-lg
            pl-1
            whitespace-nowrap
            shrink-0
            font-tianshi-pink
            text-right
            style="text-shadow: rgb(158 176 217) -1px 1px 0px, rgb(183 197 223 / 55%) 1px 1px 0px;"
          >{{ player }}</span>
        </div>
      </transition>
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
