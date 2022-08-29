export type WsKey = 'songID' | 'songName' | 'artistName' | 'startTime' | 'endTime' | 'status' | 'lyric' | 'tlyric'
export type Message = Partial<Record<WsKey, string>> & Partial<{ startTimeStamp: number; endTimeStamp: number }> & { isPlaying?: boolean }
