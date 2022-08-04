<script setup lang="ts">
import useWebSocket from '~/hooks/useWebSocket'
const { status, message } = useWebSocket()
</script>

<template>
  <div>
    <span>状态: {{ status }}</span>
    <span>数据: {{ message }}</span>
  </div>
</template>
