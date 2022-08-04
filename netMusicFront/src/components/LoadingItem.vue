<script setup lang="ts">
</script>

<template>
  <div class="loading">
    loading...
  </div>
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
