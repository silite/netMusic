#include "pch.h"
#include "App.h"
#include <Windows.h>
#include "utils.h"

const string mainScript = R"(
	function getCtlCallbackFuncName() {
		window.addEventListener('load', () => {
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

		window.addEventListener('load', () => {
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
		const artistName = document.querySelector('.artist').innerText

		const songNameDom = document.querySelector('.f-dib')
		const subSongNameDom = songNameDom.querySelector('.s-fc4')
		let songName
		let subSongName
		if (songNameDom.children.length > 2) {
			songName = Array.from(songNameDom.children)[0].innerText
			subSongName = subSongNameDom && subSongNameDom.innerText
		} else {
			songName = songNameDom.innerText
			subSongName = subSongNameDom && subSongNameDom.innerText
		}
		
		if (subSongName) songName = songName.split(subSongName).join('')

		emitMessage({ songID })
		emitMessage({ songName })
		emitMessage({ subSongName })
		emitMessage({ artistName })
	}

	const config = { childList: true, subtree: true }
	function injectSongInfo() {
		window.addEventListener('load', () => {
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
		window.addEventListener('load', () => {
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
					emitMessage({ type: 'heartBeat' })
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
	}
	function main() {
		getCtlCallbackFuncName();
		initWebsocket();
		injectPlayState();
		injectSongInfo();
		injectSongProcess();
	}

	main()
)";

App::App() {
	EasyCEFHooks::onKeyEvent = [](auto client, auto browser, auto event) {
		if (event->type == KEYEVENT_KEYUP && event->windows_key_code == 123) {
			auto cef_browser_host = browser->get_host(browser);
			CefWindowInfo windowInfo{};
			CefBrowserSettings settings{};
			CefPoint point{};
			windowInfo.SetAsPopup(NULL, "EasyCEFInject DevTools");

			cef_browser_host->show_dev_tools(cef_browser_host, &windowInfo, client, &settings, &point);
		}
	};

	EasyCEFHooks::onLoadStart = [](_cef_browser_t* browser, _cef_frame_t* frame, auto transition_type) {
		if (frame->is_main(frame) && frame->is_valid(frame)) {
			wstring url = frame->get_url(frame)->str;
			EasyCEFHooks::executeJavaScript(frame, reconnectWebsocket + mainScript);
		}
	};

	EasyCEFHooks::onAddCommandLine = [&](string arg) {
		return pystring::index(arg, "disable-gpu") == -1;
	};

	EasyCEFHooks::InstallHooks();
}
App::~App() {
	EasyCEFHooks::UninstallHook();
}
