export type WsKey = 'title' | 'isPlaying' | 'playTime' | 'totalTime' | 'id' | 'lyric' | 'tlyric'
export type Message = Partial<Record<WsKey, string>>
