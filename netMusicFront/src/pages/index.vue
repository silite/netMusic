<script setup lang="ts">
import useWebSocket from '~/hooks/useWebSocket'
import useProxyMessage from '~/hooks/useProxyMessage'
import useLrc from '~/hooks/useLrc'

const { status, message } = useWebSocket()

const { proxyMessage } = await useProxyMessage(message)

const { currentLrc } = useLrc(proxyMessage)

const isPlaying = computed(() => proxyMessage.isPlaying?.includes('True'))

const getLrcItem = (str?: string) => str?.split('*ss*').filter(item => Boolean(item)) || ''
const lrc = computed(() => [getLrcItem(currentLrc.value.text?.curr), getLrcItem(currentLrc.value.text?.next)].filter(item => item?.length))
</script>

<template>
  <transition name="fade">
    <div v-if="status !== 'success'" absolute>
      <div class="loading">
        loading...
      </div>
    </div>
    <div v-else absolute>
      <div>
        lyric:
        <div v-for="(item, index) in lrc" :key="index">
          {{ item }}
        </div>
      </div>
      <div>
        name: {{ proxyMessage.title }}
      </div>
      <div>
        isPlaying: {{ isPlaying }}
      </div>
      <div>
        playTime: {{ proxyMessage.playTime }}
      </div>
      <div>
        totalTime: {{ proxyMessage.totalTime }}
      </div>
    </div>
  </transition>
</template>

<style scoped>
.loading {
  animation: breath 3s infinite alternate;
  animation-timing-function: ease-in-out;
}

@keyframes breath {
  from {
    opacity: 0.5;
  }

  70% {
    opacity: 1;
  }

  to {
    opacity: 0.5;
  }
}
</style>

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
