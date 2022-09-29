use std::fmt::Display;

use libc::*;
use winapi::{
    shared::windef::{HMENU, HWND},
    um::winuser::MSG,
};

pub type CallbackFn<T = c_void, R = c_void> = unsafe extern "stdcall" fn(*mut T) -> R;

#[repr(C)]
pub struct CEFBase {
    pub size: size_t,
    pub add_ref: CallbackFn<CEFBase>,
    pub release: CallbackFn<CEFBase, c_int>,
    pub has_one_ref: CallbackFn<CEFBase, c_int>,
    pub has_at_least_one_ref: CallbackFn<CEFBase, c_int>,
}

#[repr(C)]
#[derive(Debug, PartialEq, Eq)]
pub enum CEFTransitionType {
    Link = 0,
    Explicit = 1,
    AutoSubframe = 3,
    ManualSubframe = 4,
    FormSubmit = 7,
    Reload = 8,
    SourceMask = 0xFF,
    BlockedFlag = 0x00800000,
    ForwardBackFlag = 0x01000000,
    DirectLoadFlag = 0x02000000,
    ChainStartFlag = 0x10000000,
    ChainEndFlag = 0x20000000,
    ClientRedirectFlag = 0x40000000,
    // TT_SERVER_REDIRECT_FLAG = 0x80000000,
    // TT_IS_REDIRECT_MASK = 0xC0000000,
    // TT_QUALIFIER_MASK = 0xFFFFFF00,
}

#[repr(C)]
pub struct CEFLoadHandler {
    pub base: CEFBase,
    pub(crate) on_loading_state_change: CallbackFn, // Placeholder
    pub on_load_start: unsafe extern "stdcall" fn(
        *mut CEFLoadHandler,
        *mut CEFBrowser,
        *mut CEFFrame,
        CEFTransitionType,
    ),
}

#[repr(C)]
#[derive(Debug, PartialEq, Eq)]
pub enum CEFKeyEventType {
    RawKeyDown = 0,
    KeyDown = 1,
    KeyUp = 2,
    Char = 3,
}

#[repr(C)]
pub struct CEFKeyEvent {
    pub event_type: CEFKeyEventType,
    pub modifiers: c_uint,
    pub windows_key_code: c_int,
    pub native_key_code: c_int,
    pub is_system_key: c_int,
    pub character: wchar_t,
    pub unmodified_character: wchar_t,
    pub focus_on_editable_field: c_int,
}

#[repr(C)]
pub struct CEFKeyBoardHandler {
    pub base: CEFBase,
    pub(crate) on_pre_key_event: CallbackFn, // Placeholder
    pub on_key_event: unsafe extern "stdcall" fn(
        *mut CEFKeyBoardHandler,
        *mut CEFBrowser,
        *const CEFKeyEvent,
        *mut MSG,
    ) -> c_int,
}

#[repr(C)]
pub struct CEFString {
    string: *mut wchar_t,
    length: size_t,
    dtor: Option<unsafe extern "stdcall" fn(string: *mut wchar_t) -> c_void>,
}

impl Display for CEFString {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        if self.string.is_null() {
            f.write_str("")
        } else {
            unsafe {
                f.write_str(&String::from_utf16_lossy(std::slice::from_raw_parts(self.string, self.length)))
            }
        }
    }
}

impl Default for CEFString {
    fn default() -> Self {
        unsafe { std::mem::zeroed() }
    }
}

impl From<CEFString> for String {
    fn from(data: CEFString) -> Self {
        if data.string.is_null() {
            "".into()
        } else {
            unsafe {
                String::from_utf16_lossy(std::slice::from_raw_parts(data.string, data.length))
            }
        }
    }
}

impl From<&str> for CEFString {
    fn from(data: &str) -> Self {
        unsafe {
            let mut result = Default::default();
            static mut CEF_STRING_UTF16_SET_ADDR: usize = 0;
            if CEF_STRING_UTF16_SET_ADDR == 0 {
                CEF_STRING_UTF16_SET_ADDR =
                    crate::get_module_symbol_address("libcef.dll", "cef_string_utf16_set")
                        .expect("Can't get cef_string_utf16_set");
            }
            let cef_string_utf16_set: unsafe extern "cdecl" fn(
                *const wchar_t,
                size_t,
                *mut Self,
                c_int,
            ) -> c_int = std::mem::transmute(CEF_STRING_UTF16_SET_ADDR);
            let data = data.encode_utf16().chain(Some(0)).collect::<Vec<_>>();
            cef_string_utf16_set(data.as_ptr(), data.len() - 1, &mut result, 1);
            result
        }
    }
}

#[repr(C)]
pub struct CEFWindowInfo {
    pub ex_style: u32,
    pub window_name: CEFString,
    pub style: u32,
    pub x: c_int,
    pub y: c_int,
    pub width: c_int,
    pub height: c_int,
    pub parent_window: HWND,
    pub menu: HMENU,
    pub windowless_rendering_enabled: c_int,
    pub shared_texture_enabled: c_int,
    pub external_begin_frame_enabled: c_int,
    pub window: HWND,
}

#[repr(C)]
pub enum CEFState {
    StateDefault = 0,
    StateEnabled = 1,
    StateDisabled = 2,
}

#[repr(C)]
pub struct CEFBrowserSettings {
    pub size: size_t,
    pub windowless_frame_rate: c_int,
    pub standard_font_family: CEFString,
    pub fixed_font_family: CEFString,
    pub serif_font_family: CEFString,
    pub sans_serif_font_family: CEFString,
    pub cursive_font_family: CEFString,
    pub fantasy_font_family: CEFString,
    pub default_font_size: c_int,
    pub default_fixed_font_size: c_int,
    pub minimum_font_size: c_int,
    pub minimum_logical_font_size: c_int,
    pub default_encoding: CEFString,
    pub remote_fonts: CEFState,
    pub javascript: CEFState,
    pub javascript_close_windows: CEFState,
    pub javascript_access_clipboard: CEFState,
    pub javascript_dom_paste: CEFState,
    pub plugins: CEFState,
    pub universal_access_from_file_urls: CEFState,
    pub file_access_from_file_urls: CEFState,
    pub image_loading: CEFState,
    pub image_shrink_standalone_to_fit: CEFState,
    pub text_area_resize: CEFState,
    pub tab_to_links: CEFState,
    pub local_storage: CEFState,
    pub databases: CEFState,
    pub application_cache: CEFState,
    pub webgl: CEFState,
    pub background_color: c_uint,
    pub accept_language_list: CEFString,
}

#[repr(C)]
pub struct CEFPoint {
    pub x: c_int,
    pub y: c_int,
}

#[repr(C)]
pub struct CEFBrowserHost {
    pub base: CEFBase,
    pub get_browser: unsafe extern "stdcall" fn(*mut CEFBrowserHost) -> *mut CEFBrowser,
    pub(crate) close_browser: CallbackFn,     // Placeholder
    pub(crate) try_close_browser: CallbackFn, // Placeholder
    pub(crate) set_focus: CallbackFn,         // Placeholder
    pub(crate) get_window_handle: CallbackFn, // Placeholder
    pub(crate) get_opener_window_handle: CallbackFn, // Placeholder
    pub(crate) has_view: CallbackFn,          // Placeholder
    pub(crate) get_client: CallbackFn,        // Placeholder
    pub(crate) get_request_context: CallbackFn, // Placeholder
    pub(crate) get_zoom_level: CallbackFn,    // Placeholder
    pub(crate) set_zoom_level: CallbackFn,    // Placeholder
    pub(crate) run_file_dialog: CallbackFn,   // Placeholder
    pub(crate) start_download: CallbackFn,    // Placeholder
    pub(crate) download_image: CallbackFn,    // Placeholder
    pub(crate) print: CallbackFn,             // Placeholder
    pub(crate) print_to_pdf: CallbackFn,      // Placeholder
    pub(crate) find: CallbackFn,              // Placeholder
    pub(crate) stop_finding: CallbackFn,      // Placeholder
    pub show_dev_tools: unsafe extern "stdcall" fn(
        *mut CEFBrowserHost,
        *const CEFWindowInfo,
        *mut CEFClient,
        *mut CEFBrowserSettings,
        *const CEFPoint,
    ) -> c_void, // Placeholder
    pub close_dev_tools: unsafe extern "stdcall" fn(*mut CEFBrowserHost) -> c_void, // Placeholder
    pub has_dev_tools: unsafe extern "stdcall" fn(*mut CEFBrowserHost) -> c_int, // Placeholder
}

#[repr(C)]
pub struct CEFBrowser {
    pub base: CEFBase,
    pub get_host: unsafe extern "stdcall" fn(*mut CEFBrowser) -> *mut CEFBrowserHost, // Placeholder
    pub(crate) can_go_back: CallbackFn,                                               // Placeholder
    pub(crate) go_back: CallbackFn,                                                   // Placeholder
    pub(crate) can_go_forward: CallbackFn,                                            // Placeholder
    pub(crate) go_forward: CallbackFn,                                                // Placeholder
    pub(crate) is_loading: CallbackFn,                                                // Placeholder
    pub reload: unsafe extern "stdcall" fn(*mut CEFBrowser) -> c_void,                // Placeholder
    pub(crate) reload_ignore_cache: CallbackFn,                                       // Placeholder
    pub(crate) stop_load: CallbackFn,                                                 // Placeholder
    pub(crate) get_identifier: CallbackFn,                                            // Placeholder
    pub(crate) is_same: CallbackFn,                                                   // Placeholder
    pub(crate) is_popup: CallbackFn,                                                  // Placeholder
    pub(crate) has_document: CallbackFn,                                              // Placeholder
    pub(crate) get_main_frame: CallbackFn,                                            // Placeholder
    pub(crate) get_focused_frame: CallbackFn,                                         // Placeholder
    pub(crate) get_frame_byident: CallbackFn,                                         // Placeholder
    pub(crate) get_frame: CallbackFn,                                                 // Placeholder
    pub(crate) get_frame_count: CallbackFn,                                           // Placeholder
    pub(crate) get_frame_identifiers: CallbackFn,                                     // Placeholder
    pub(crate) get_frame_names: CallbackFn,                                           // Placeholder
}

impl CEFBrowser {
    pub fn get_host(&mut self) -> *mut CEFBrowserHost {
        unsafe { (self.get_host)(self) }
    }

    pub fn reload(&mut self) {
        unsafe {
            (self.reload)(self);
        }
    }
}

#[repr(C)]
pub struct CEFV8Value {
    pub base: CEFBase,
    pub(crate) is_valid: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) is_undefined: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) is_null: CallbackFn<CEFV8Value, c_int>,  // Placeholder
    pub(crate) is_bool: CallbackFn<CEFV8Value, c_int>,  // Placeholder
    pub(crate) is_int: CallbackFn<CEFV8Value, c_int>,   // Placeholder
    pub(crate) is_uint: CallbackFn<CEFV8Value, c_int>,  // Placeholder
    pub(crate) is_double: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) is_date: CallbackFn<CEFV8Value, c_int>,  // Placeholder
    pub(crate) is_string: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) is_object: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) is_array: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) is_array_buffer: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) is_function: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) is_same: CallbackFn<CEFV8Value, c_int>,  // Placeholder
    pub(crate) get_bool_value: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_int_value: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_uint_value: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_double_value: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_date_value: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_string_value: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) is_user_created: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) has_exception: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_exception: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) clear_exception: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) will_rethrow_exceptions: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) set_rethrow_exceptions: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) has_value_bykey: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) has_value_byindex: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) delete_value_bykey: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) delete_value_byindex: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_value_bykey: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_value_byindex: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) set_value_bykey: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) set_value_byindex: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) set_value_byaccessor: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_keys: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) set_user_data: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_user_data: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_externally_allocated_memory: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) adjust_externally_allocated_memory: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_array_length: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_array_buffer_release_callback: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) neuter_array_buffer: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_function_name: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) get_function_handler: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) execute_function: CallbackFn<CEFV8Value, c_int>, // Placeholder
    pub(crate) execute_function_with_context: CallbackFn<CEFV8Value, c_int>, // Placeholder
}

impl CEFV8Value {
    #[doc = ""]
    pub unsafe fn new_undefined() -> *mut Self {
        static mut ADDR: usize = 0;
        if ADDR == 0 {
            ADDR = crate::get_module_symbol_address("libcef.dll", "cef_v8value_create_undefined")
                .expect("Can't get cef_v8value_create_undefined");
        }
        let cef_v8value_create_undefined: unsafe extern "cdecl" fn() -> *mut CEFV8Value =
            std::mem::transmute(ADDR);
        cef_v8value_create_undefined()
    }

    #[doc = ""]
    pub unsafe fn new_null() -> *mut Self {
        static mut ADDR: usize = 0;
        if ADDR == 0 {
            ADDR = crate::get_module_symbol_address("libcef.dll", "cef_v8value_create_null")
                .expect("Can't get cef_v8value_create_undefined");
        }
        let cef_v8value_create_null: unsafe extern "cdecl" fn() -> *mut CEFV8Value =
            std::mem::transmute(ADDR);
        cef_v8value_create_null()
    }

    #[doc = ""]
    pub unsafe fn new_function() -> *mut Self {
        static mut ADDR: usize = 0;
        if ADDR == 0 {
            ADDR = crate::get_module_symbol_address("libcef.dll", "cef_v8value_create_function")
                .expect("Can't get cef_v8value_create_undefined");
        }
        let cef_v8value_create_function: unsafe extern "cdecl" fn() -> *mut CEFV8Value =
            std::mem::transmute(ADDR);
        cef_v8value_create_function()
    }
}

#[repr(C)]
pub struct CEFV8Context {
    pub base: CEFBase,
    pub(crate) get_task_runner: CallbackFn, // Placeholder
    pub(crate) get_valid: CallbackFn,       // Placeholder
    pub(crate) get_browser: CallbackFn,     // Placeholder
    pub(crate) get_frame: CallbackFn,       // Placeholder
    pub(crate) get_global: unsafe extern "stdcall" fn(*mut CEFV8Context) -> *mut CEFV8Value, // Placeholder
    pub(crate) enter: CallbackFn,   // Placeholder
    pub(crate) exit: CallbackFn,    // Placeholder
    pub(crate) is_same: CallbackFn, // Placeholder
    pub(crate) eval: CallbackFn,    // Placeholder
}

#[repr(C)]
pub struct CEFFrame {
    pub base: CEFBase,
    pub(crate) is_valid: CallbackFn,     // Placeholder
    pub(crate) undo: CallbackFn,         // Placeholder
    pub(crate) redo: CallbackFn,         // Placeholder
    pub(crate) cut: CallbackFn,          // Placeholder
    pub(crate) copy: CallbackFn,         // Placeholder
    pub(crate) paste: CallbackFn,        // Placeholder
    pub(crate) del: CallbackFn,          // Placeholder
    pub(crate) select_all: CallbackFn,   // Placeholder
    pub(crate) view_source: CallbackFn,  // Placeholder
    pub(crate) get_source: CallbackFn,   // Placeholder
    pub(crate) get_text: CallbackFn,     // Placeholder
    pub(crate) load_request: CallbackFn, // Placeholder
    pub(crate) load_url: CallbackFn,     // Placeholder
    pub execute_java_script:
        unsafe extern "stdcall" fn(*mut CEFFrame, *mut CEFString, *mut CEFString, c_int) -> c_void,
    pub is_main: unsafe extern "stdcall" fn(*mut CEFFrame) -> c_int, // Placeholder
    pub(crate) is_focused: CallbackFn,                               // Placeholder
    pub(crate) get_name: CallbackFn,                                 // Placeholder
    pub(crate) get_identifier: CallbackFn,                           // Placeholder
    pub(crate) get_parent: CallbackFn,                               // Placeholder
    pub(crate) get_url: CallbackFn,                                  // Placeholder
    pub get_browser: unsafe extern "stdcall" fn(*mut CEFFrame) -> *mut CEFBrowser,
    pub get_v8context: unsafe extern "stdcall" fn(*mut CEFFrame) -> *mut CEFV8Context,
}

impl CEFFrame {
    pub fn is_main(&mut self) -> bool {
        unsafe { (self.is_main)(self) != 0 }
    }

    pub fn execute_java_script(&mut self, code: &str) {
        unsafe {
            (self.execute_java_script)(self, &mut code.into(), &mut "".into(), 0);
        }
    }

    pub fn get_v8context(&mut self) -> *mut CEFV8Context {
        unsafe { (self.get_v8context)(self) }
    }
}

#[repr(C)]
pub struct CEFClient {
    pub base: CEFBase,
    pub(crate) get_audio_handler: CallbackFn, // Placeholder
    pub(crate) get_context_menu_handler: CallbackFn, // Placeholder
    pub(crate) get_dialog_handler: CallbackFn, // Placeholder
    pub(crate) get_display_handler: CallbackFn, // Placeholder
    pub(crate) get_download_handler: CallbackFn, // Placeholder
    pub(crate) get_drag_handler: CallbackFn,  // Placeholder
    pub(crate) get_find_handler: CallbackFn,  // Placeholder
    pub(crate) get_focus_handler: CallbackFn, // Placeholder
    pub(crate) get_jsdialog_handler: CallbackFn, // Placeholder
    pub get_keyboard_handler: CallbackFn<CEFClient, *mut CEFKeyBoardHandler>,
    pub(crate) get_life_span_handler: CallbackFn, // Placeholder
    pub get_load_handler: CallbackFn<CEFClient, *mut CEFLoadHandler>,
}

#[repr(C)]
pub struct CEFCommandLine {
    pub base: CEFBase,
    pub(crate) is_valid: CallbackFn,                 // Placeholder
    pub(crate) is_read_only: CallbackFn,             // Placeholder
    pub(crate) copy: CallbackFn,                     // Placeholder
    pub(crate) init_from_argv: CallbackFn,           // Placeholder
    pub(crate) init_from_string: CallbackFn,         // Placeholder
    pub(crate) reset: CallbackFn,                    // Placeholder
    pub(crate) get_argv: CallbackFn,                 // Placeholder
    pub(crate) get_command_line_string: CallbackFn,  // Placeholder
    pub(crate) get_program: CallbackFn,              // Placeholder
    pub(crate) set_program: CallbackFn,              // Placeholder
    pub(crate) has_switches: CallbackFn,             // Placeholder
    pub(crate) has_switch: CallbackFn,               // Placeholder
    pub(crate) get_switch_value: CallbackFn,         // Placeholder
    pub(crate) get_switches: CallbackFn,             // Placeholder
    pub(crate) append_switch: unsafe extern "stdcall" fn(*mut CEFCommandLine, *const CEFString),            // Placeholder
    pub(crate) append_switch_with_value: CallbackFn, // Placeholder
    pub(crate) has_arguments: CallbackFn,            // Placeholder
    pub(crate) get_arguments: CallbackFn,            // Placeholder
    pub(crate) append_argument: CallbackFn,          // Placeholder
    pub(crate) prepend_wrapper: CallbackFn,          // Placeholder
}
#[repr(C)]
pub struct CEFApp {
    pub base: CEFBase,
    pub on_before_command_line_processing:
        unsafe extern "stdcall" fn(*mut CEFApp, *const CEFString, *mut CEFCommandLine),
}
