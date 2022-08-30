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
</script>

<template>
  <div flex>
    <div flex="~ col" items-center justify-end pr-20px>
      <span text-lg>{{ lrcProxy.currOLrc }}</span>
      <span text-sm>{{ lrcProxy.currTLrc }}</span>
    </div>
    <div flex="~ col" text-xs items-center justify-end opacity-60>
      <span>{{ lrcProxy.nextOLrc }}</span>
      <span>{{ lrcProxy.nextTLrc }}</span>
    </div>
    <div flex="~ col" text-xs items-center justify-end opacity-60>
      <span>{{ lrcProxy.nextOLrc }}</span>
      <span>{{ lrcProxy.nextTLrc }}</span>
    </div>
  </div>
</template>
