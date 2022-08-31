<script setup lang="ts">
import useWebSocket from '~/hooks/useWebSocket'
import useProxyMessage from '~/hooks/useProxyMessage'
import useLrc from '~/hooks/useLrc'
import useProgress from '~/hooks/useProgress'

const { message } = useWebSocket()

const { proxyMessage } = await useProxyMessage(message)

const lrc = useLrc(proxyMessage)

const progress = useProgress(proxyMessage)
</script>

<template>
  <transition name="fade">
    <div v-if="message.lrcStatus">
    <div v-if="message.status !== 'start'" absolute>
      <LoadingItem />
    </div>
    <div v-else absolute>
      <SongItem
        :songName="proxyMessage.songName"
        :subSongName="proxyMessage.subSongName"
        :player="proxyMessage.artistName"
        :progress="progress"
        :isPlaying="proxyMessage.isPlaying"
      />
      <LrcItem :lrc="lrc" />
    </div>
    </div>
  </transition>
</template>

<style scoped>
.fade-enter-active,
.fade-leave-active {
  transition: opacity 0.5s ease;
}

.fade-enter-from,
.fade-leave-to {
  opacity: 0;
}
</style>
