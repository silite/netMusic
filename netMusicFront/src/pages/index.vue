<script setup lang="ts">
import useWebSocket from '~/hooks/useWebSocket'
import useProxyMessage from '~/hooks/useProxyMessage'

const { status, message } = useWebSocket()

const { proxyMessage } = useProxyMessage(message)
</script>

<template>
  <div>
    <span>状态: {{ status }}</span>
    <span>数据: {{ proxyMessage }}</span>
  </div>
</template>
