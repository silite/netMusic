use std::sync::atomic::{AtomicBool, Ordering};

use easycefhooks::types::*;

const EAPI_SHARK: &str = include_str!("../resource/core/eapi-shark.js");
const LIVE_SRC: &str = include_str!("../../front-bundle/dist/main.js");


static SERVER_INITIALIZED: AtomicBool = AtomicBool::new(false);

fn dynamic_load_script(f: &mut CEFFrame, path: &str) {
    if let Ok(script) = std::fs::read_to_string(path) {
        println!("Running dynamic script at {}", path);
        f.execute_java_script(&script);
    }
}

// Do Hook Stuff here
pub(crate) fn install_hook() {
    #[cfg(debug_assertions)]
    unsafe {
        winapi::um::consoleapi::AllocConsole();
    }
    
    println!("BetterNCM Started!");

    easycefhooks::install_hooks();
    println!("BetterNCM Hooked!");

    if !SERVER_INITIALIZED.swap(true, Ordering::SeqCst) {
        println!("Initializing Server");
        crate::server::init_server();
    }

    fn on_load(_b: &mut CEFBrowser, f: &mut CEFFrame, _tt: CEFTransitionType) {
        if f.is_main() {
            println!("Running EAPI Shark Script");
            f.execute_java_script(EAPI_SHARK);
            f.execute_java_script(LIVE_SRC);
        }
    }

    easycefhooks::set_on_load_handler(on_load);
}

pub(crate) fn uninstall_hook() {
    if SERVER_INITIALIZED.swap(true, Ordering::SeqCst) {
        crate::server::uninit_server();
    }
}
