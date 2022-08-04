<script setup lang="ts">
import ReconnectReconnectingWebSocket from 'reconnecting-websocket'
type wsKey = 'title' | 'isPlaying' | 'playTime' | 'totalTime' | 'id'

const status = ref('loading')
const message = reactive<Partial<Record<wsKey, string>>>({})

const rws = new ReconnectReconnectingWebSocket('ws://localhost:12449')

rws.addEventListener('open', () => {
  status.value = 'connected'
})

rws.addEventListener('message', (evt) => {
  const data = evt.data as string

  status.value = data.includes('error') ? 'error' : 'success'

  const [key, value] = data.split('*ss*')
  message[key as wsKey] = value
})

rws.addEventListener('close', () => {
  status.value = 'loading'
})
</script>

<template>
  <div>
    <span>状态: {{ status }}</span>
    <span>数据: {{ message }}</span>
  </div>
</template>
