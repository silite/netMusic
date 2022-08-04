export type WsKey = 'title' | 'isPlaying' | 'playTime' | 'totalTime' | 'id'
export type Message = Partial<Record<WsKey, string>>
