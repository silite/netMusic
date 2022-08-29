<script setup lang="ts">
interface propType {
  songName?: string
  player?: string
  progress?: number
  isPlaying?: boolean
  subSongName?: string
}
const props = defineProps<propType>()

const songNameRef = ref()
const songNameWidth = ref(0)
watch(() => props.songName, () => {
  nextTick(() => {
    songNameWidth.value = songNameRef.value?.offsetWidth
  })
}, { immediate: true })

const subSongNameStyle = computed(() => ({ width: `${songNameWidth.value}px` }))
</script>

<template>
  <div
    flex="~"
    w-1200px
    h-400px
  >
    <div flex="~ col" items-end>
      <span
        ref="songNameRef"
        text-xl
        font="bold tianshi-blue">
        {{ songName }}
      </span>
      <span
        :style="subSongNameStyle"
        text-xs
        color-violet-400
        whitespace-nowrap
        overflow-hidden
      >
      {{ subSongName }}
      </span>
      <span text-lg font-tianshi-pink>{{ player }}</span>
    </div>
    <div>
      <span>进度条:{{ progress || 0 }}</span>
      <span>{{ isPlaying }}</span>
    </div>
  </div>
</template>
