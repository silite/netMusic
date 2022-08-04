<script setup lang="ts">
import useWebSocket from '~/hooks/useWebSocket'
import useProxyMessage from '~/hooks/useProxyMessage'
import useLrc from '~/hooks/useLrc'

const { status, message } = useWebSocket()

const { proxyMessage } = await useProxyMessage(message)

const { currentLrc } = useLrc(proxyMessage)
</script>

<template>
  <div>
    <span>状态: {{ status }}</span>
    <div>
      lyric: {{ currentLrc?.text?.split('*ss*') }}
    </div>
    <div>
      name: {{ proxyMessage.title }}
    </div>
    <div>
      isPlaying: {{ proxyMessage.isPlaying }}
    </div>
    <div>
      playTime: {{ proxyMessage.playTime }}
    </div>
    <div>
      totalTime: {{ proxyMessage.totalTime }}
    </div>
  </div>
</template>
