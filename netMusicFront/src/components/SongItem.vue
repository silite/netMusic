<script setup lang="ts">
interface propType {
  songName?: string
  player?: string
  progress?: number
  isPlaying?: boolean
}
defineProps<propType>()
</script>

<template>
  <div flex="~">
    <div>
      <span>进度条:{{ progress }}</span>
      <span>{{ isPlaying }}</span>
    </div>
    <div flex="~ col" items-end>
      <span font="bold">{{ songName }}</span>
      <span text-xs>{{ player }}</span>
    </div>
  </div>
</template>
