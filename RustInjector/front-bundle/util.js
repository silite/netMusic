export function addOnMessageCB(cb) {
  if (!window.onSocketMsgCB)
    window.onSocketMsgCB = []
  window.onSocketMsgCB.push(cb)
}

export function getCtlCallbackFuncName(funcName, mountName) {
  window.addEventListener('DOMContentLoaded', () => {
    for (let [key, val] of Object.entries(ctl.cefPlayer)) {
      if (val instanceof Object && Object.keys(val).includes(funcName))
        window[mountName] = key
    }
  })
}
