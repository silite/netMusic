#include "pch.h"
#include "App.h"
#include <Windows.h>

const auto mainSctipt = R"(
	window.emitMessage = (messageObj) => {
		console.log(messageObj)
	}

	function injectPlayState() {
		let isPlaying = false

		window.addEventListener('load', () => {
			Object.defineProperty(window.player, 'playState', {
				set(newValue) {
					triggerPlayState(newValue)
					val = newValue
				}
			})
			emitMessage({ isPlaying })
		})

		const triggerPlayState = (playState = '') => {
			isPlaying = playState === 'play'
			emitMessage({ isPlaying })
		}
	}

	function injectSongId() {
		window.addEventListener('load', () => {
			const config = { childList: true, subtree: true }
			let observerBody;
			observerBody = new MutationObserver(() => {
				const parentDom = document.querySelector('.info')
				if (parentDom) {
					observerBody.disconnect();
					
					const observerCallback = (mutationsList, observer) => {
						const songID = document.querySelector('.btn-love').getAttribute('data-res-id')
						const songName = document.querySelector('.f-dib').innerText
						const artistName = document.querySelector('.artist').innerText

						emitMessage({ songID })
						emitMessage({ songName })
						emitMessage({ artistName })
					};

					(new MutationObserver(observerCallback)).observe(parentDom, config)
				}
			})
			observerBody.observe(document.body, config)
		})
	}

	function injectSongName() {

	}

	injectPlayState();
	injectSongId();
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
			EasyCEFHooks::executeJavaScript(frame, mainSctipt);
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
