export const str2Time = (str?: string) => {
  if (!str)
    return 0
  const [min, seconds] = str.split(':')
  return (+min || 0) * 60 + (+seconds || 0)
}

export const time2Str = (time: number) => {
  return `${Math.floor(time / 60)}:${time % 60 < 10 ? '0' : ''}${time % 60}`
}
