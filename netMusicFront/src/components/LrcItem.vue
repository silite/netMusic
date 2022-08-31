<script setup lang="ts">
interface propsType {
  lrc?: (string | string[])[]
}
const props = defineProps<propsType>()
const { lrc } = toRefs(props)

const lrcProxy = ref<Partial<Record<'currOLrc' | 'currTLrc' | 'nextOLrc' | 'nextTLrc', string>>>({})
watchEffect(() => {
  const [currLrc, nextLrc] = lrc?.value || []
  const [currOLrc, currTLrc] = currLrc || []
  const [nextOLrc, nextTLrc] = nextLrc || []
  lrcProxy.value = { currOLrc, currTLrc, nextOLrc, nextTLrc }
})

const triggerAnimate = () => {}
watch(() => lrc, () => {
  triggerAnimate()
}, { deep: true })

const nextLrcShow = computed(() => {
  return Math.max(lrcProxy.value.currOLrc?.length || 0, lrcProxy.value.currTLrc?.length || 0) <= 20
})
</script>

<template>
  <div flex mt-25px justify-center>
    <div flex="~ col" items-center justify-end pr-20px>
      <span text-xl whitespace-nowrap style="color: #74a0f7; text-shadow: rgb(255 255 255) -1px 1px 0px, rgb(169 82 118 / 55%) 1px 1px 0px;">{{ lrcProxy.currOLrc }}</span>
      <span text-base whitespace-nowrap style="color: rgb(168 195 247);">{{ lrcProxy.currTLrc }}</span>
    </div>
    <div v-if="nextLrcShow" flex="~ col" items-center justify-end opacity-80>
      <span text-sm whitespace-nowrap style="color: #74a0f7;">{{ lrcProxy.nextOLrc }}</span>
      <span text-xs whitespace-nowrap style="color: rgb(168 195 247);">{{ lrcProxy.nextTLrc }}</span>
    </div>
  </div>
</template>
