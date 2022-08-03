<script setup lang="ts">
type wsKey = 'title' | 'isPlaying' | 'playTime' | 'totalTime' | 'id'

const status = ref('loading')
const message = reactive<Partial<Record<wsKey, string>>>({})

let ws: WebSocket | null
const wsInit = () => {
  ws = new WebSocket('ws://localhost:12449')

  ws.onopen = () => {
    status.value = 'connected'
  }

  ws.onmessage = (evt) => {
    const data = evt.data as string
    if (data.includes('error')) {
      status.value = 'error'
    }
    else if (data) {
      const [key, value] = data.split('*ss*')
      message[key as wsKey] = value
    }
  }

  ws.onerror = () => {
    status.value = 'loading'
    setTimeout(() => {
      ws?.close()
      ws = null
      wsInit()
    }, 1000)
  }

  ws.onclose = () => {
    status.value = 'loading'
    setTimeout(() => {
      ws?.close()
      ws = null
      wsInit()
    }, 1000)
  }
}

wsInit()
</script>

<template>
  <div>
    <span>状态: {{ status }}</span>
    <span>数据: {{ message }}</span>
  </div>
</template>
