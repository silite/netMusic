<script setup lang="ts">
</script>

<template>
  <div
    class="loading"
    flex="~"
    w-600px
    h-200px
    items-center
    justify-center
    text-2xl
    tracking-widest
  >
   loading...
  </div>
</template>

<style scoped>
.loading {
  animation: breath 5s infinite alternate;
  animation-timing-function: ease-in-out;

  color: rgb(209, 149, 221);
}

@keyframes breath {
  from {
    opacity: 0.4;
  }

  70% {
    opacity: 1;
  }

  to {
    opacity: 0.4;
  }
}
</style>
