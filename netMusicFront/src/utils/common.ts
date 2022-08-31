export const str2Time = (str?: string) => {
  if (!str)
    return 0
  const [min, seconds] = str.split(':')
  return (+min || 0) * 60 + (+seconds || 0)
}

export const time2Str = (time: number) => {
  return `${Math.floor(time / 60)}:${time % 60 < 10 ? '0' : ''}${time % 60}`
}

export const getDomWidth = (text: string, style: CSSStyleDeclaration): Promise<number> => {
  return new Promise((resolve) => {
    const container = document.createElement('div')
    container.innerText = text
    for (const [key, value] of Object.entries(style))
      (container.style as any)[key] = value
    container.style.opacity = '0'
    const zpix = new FontFace('zpix', 'url(zpix.woff2)')
    zpix.load().then(() => {
      container.style.fontFamily = 'zpix'
      container.style.display = 'inline-block'
      document.body.appendChild(container)
      const width = container.offsetWidth
      container.remove()
      resolve(width)
    })
  })
}
