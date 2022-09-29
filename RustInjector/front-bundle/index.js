import './style-script'
import ReconnectingWebSocket from 'reconnecting-websocket'

function getCtlCallbackFuncName() {
    window.addEventListener('DOMContentLoaded', () => {
        for (let [key, val] of Object.entries(ctl.cefPlayer)) {
            if (val instanceof Object && Object.keys(val).includes('onpositionchange'))
                window.cefCallBackFuncName = key
        }
    })
}

function emitPlayState() {
    emitMessage({ isPlaying: window.playerProxy === 'play' })
}
function injectPlayState() {
    window.playerProxy = 'stop'

    window.addEventListener('DOMContentLoaded', () => {
        Object.defineProperty(window.player, 'playState', {
            get() {
                return window.playerProxy || {}
            },
            set(newValue) {
                window.playerProxy = newValue
                emitPlayState()
            }
        })
        emitPlayState()
    })
}

function emitSongInfo() {
    const songID = document.querySelector('.btn-love').getAttribute('data-res-id')
    const artistDom = document.querySelectorAll('.j-title')[1].children[0]

    const songNameDom = document.querySelector('.f-dib')
    const subSongNameDom = songNameDom.querySelector('.s-fc4')
    let songName
    let subSongName
    if (songNameDom.children.length > 2) {
        songName = songNameDom.children[0].innerText
        subSongName = subSongNameDom && subSongNameDom.innerText
    } else {
        songName = songNameDom.innerText
        subSongName = subSongNameDom && subSongNameDom.innerText
    }

    let artistName
    if (artistDom.children.length > 2) {
        artistName = artistDom.children[0].innerText
    } else {
        artistName = artistDom.innerText
    }

    if (subSongName) songName = songName.split(subSongName).join('')

    emitMessage({ songID })
    emitMessage({ songName })
    emitMessage({ subSongName })
    emitMessage({ artistName })
}

const config = { childList: true, subtree: true }
function injectSongInfo() {
    window.addEventListener('DOMContentLoaded', () => {
        let observerBody;
        observerBody = new MutationObserver(() => {
            const parentDom = document.querySelector('.info')
            if (parentDom) {
                observerBody.disconnect();

                const observerCallback = (mutationsList, observer) => {
                    emitSongInfo()
                };

                (new MutationObserver(observerCallback)).observe(parentDom, config)
            }
        })
        observerBody.observe(document.body, config)
    })
}

function injectSongProcess() {
    window.addEventListener('DOMContentLoaded', () => {
        ctl.cefPlayer[cefCallBackFuncName].onpositionchange.push(({ current, duration } = {}) => {
            emitMessage({ current })
            emitMessage({ duration })
        })
    })
}

function initWebsocket() {
    const ws = new ReconnectingWebSocket('ws://localhost:13430')

    const heartCheck = {
        timeout: 30000,
        timeoutObj: null,
        reset: function() {
            clearTimeout(this.timeoutObj)
            this.start()
        },
        start: function() {
            this.timeoutObj = setTimeout(() => {
                console.log('beat...')
                sendAllInfo()
            }, this.timeout)
        }
    }

    ws.onopen = () => {
        heartCheck.start()
    }

    ws.onmessage = (data) => {
        if (data.data === 'init') {
            sendAllInfo()
        }
    }

    window.emitMessage = (messageObj = {}) => {
        heartCheck.reset()
        ws.send(JSON.stringify({ status: 'start' }))
        ws.send(JSON.stringify(messageObj))
    }
}


function sendAllInfo() {
    emitPlayState()
    emitSongInfo()
    emitMessage({ lrcStatus: window.lrcStatus })
}
function main() {
    getCtlCallbackFuncName();
    initWebsocket();
    injectPlayState();
    injectSongInfo();
    injectSongProcess();
}

main();
