#include "pch.h"
#include "App.h"
#include <Windows.h>
#include "utils.h"

const string mainScript = R"(
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
)";

const string styleScript = R"(
	window.injectCss = function(dom, style) {
		for (let [key, value] of Object.entries(style)) {
			dom.style[key] = value
		}
	}

	console.log('test')
	window.addEventListener('DOMContentLoaded', () => {
		console.log('load')
		let observerBody;
		const trigger = () => {
			document.querySelector('.logo2').remove()
			document.querySelector('.logo1').style.paddingLeft = '10px'
			document.querySelector('.m-leftbox').style.left = '40px';
		
		
			const lrcBotSwitch = createSwitch()
			const switchRef = lrcBotSwitch.querySelector('input')
			switchRef.checked = window.lrcStatus = true
			emitMessage({ lrcStatus: true })
			lrcBotSwitch.addEventListener('click', (event) => {
				if (event.target.type !== 'checkbox') return
				const lrcStatus = window.lrcStatus = event.target.checked
				customToast(lrcStatus ? 'lrc start' : 'lrc stop')
				emitMessage({ lrcStatus })
			})

			injectCss(lrcBotSwitch, { paddingLeft: '10px', transform: 'scale(0.5)' })
			document.querySelector('.m-topbox.j-topbox').appendChild(lrcBotSwitch)
		}

		if (document.querySelector('.m-topbox.j-topbox')) trigger()
		
		observerBody = new MutationObserver(() => {
			if (document.querySelector('.m-topbox.j-topbox')) {
				observerBody.disconnect();

				trigger()
			}
		})
		observerBody.observe(document.body, config)
	});
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
			EasyCEFHooks::executeJavaScript(frame, reconnectWebsocket + mainScript + switchInput + customToast + styleScript);
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
