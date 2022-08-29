import {
  defineConfig,
  presetAttributify,
  presetIcons,
  presetUno,
  presetWebFonts,
  // transformerDirectives,
  // transformerVariantGroup,
} from 'unocss'

export default defineConfig({
  shortcuts: [
    ['btn', 'px-4 py-1 rounded inline-block bg-teal-600 text-white cursor-pointer hover:bg-teal-700 disabled:cursor-default disabled:bg-gray-600 disabled:opacity-50'],
    ['icon-btn', 'text-[0.9em] inline-block cursor-pointer select-none opacity-75 transition duration-200 ease-in-out hover:opacity-100 hover:text-teal-600 !outline-none'],
  ],
  rules: [
    [
      'font-custom',
      {
        'font-family': 'zpix, \'Josefin Sans\', -apple-system, \'PingFang SC\', \'Hiragino Sans GB\', \'Microsoft Yahei\', \'WenQuanYi Micro Hei\', \'ST Heiti\', sans-serif',
      },
    ],
    [
      'font-tianshi-blue',
      {
        color: '#617cc7',
      },
    ],
    [
      'font-tianshi-pink',
      {
        color: '#dfaedf',
      },
    ],
  ],
  presets: [
    presetUno(),
    presetAttributify(),
    presetIcons({
      scale: 1.2,
      warn: true,
    }),
    presetWebFonts({
      fonts: {
        sans: 'DM Sans',
        serif: 'DM Serif Display',
        mono: 'DM Mono',
      },
    }),
  ],
  // transformers: [
  //   transformerDirectives(),
  //   transformerVariantGroup(),
  // ],
})
