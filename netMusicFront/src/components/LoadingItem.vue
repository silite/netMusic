<script setup lang="ts">
</script>

<template>
  <div
    class="loading"
    flex="~"
    h-70px
    pl-2
    items-center
    justify-center
    text-2xl
    tracking-widest
    style="text-shadow: rgb(141 164 217) -1px 1px 0px, rgb(169 82 118 / 55%) 1px 1px 0px;"
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
