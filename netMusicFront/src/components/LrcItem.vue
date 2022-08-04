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
</script>

<template>
  <div>
    <div flex="~ col" items-center>
      <span>{{ lrcProxy.currOLrc }}</span>
      <span>{{ lrcProxy.currTLrc }}</span>
    </div>
    <div flex="~ col" text-xs items-center opacity-60>
      <span>{{ lrcProxy.nextOLrc }}</span>
      <span>{{ lrcProxy.nextTLrc }}</span>
    </div>
  </div>
</template>
