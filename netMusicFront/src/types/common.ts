export type WsKey = 'songID' | 'songName' | 'subSongName' | 'artistName' | 'status' | 'lyric' | 'tlyric' | 'lrcStatus'
export type Message = Partial<Record<WsKey, string>> & Partial<{ current: number; duration: number }> & { isPlaying?: boolean }
