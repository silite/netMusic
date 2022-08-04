export type WsKey = 'title' | 'isPlaying' | 'playTime' | 'totalTime' | 'id' | 'lyric'
export type Message = Partial<Record<WsKey, string>>
