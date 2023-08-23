extern crate libc;

use libc::c_char;
use std::ffi::{CString, CStr};

/**
* Public functions
*/

/// Generate a greeting
#[no_mangle]
pub extern "C" fn greeting_generate(name: *const c_char) -> *mut c_char {
    println!("[Rust] Generating greeting");
    let mut greeting = String::from("Hello, ");

    greeting.push_str(cstr_to_string(name).as_str());

    println!("[Rust] Generated greeting {}", greeting);

    let c_str_greeting = CString::new(greeting).unwrap();
    c_str_greeting.into_raw()
}

#[no_mangle]
pub extern "C" fn greeting_free(s: *mut c_char) {
    unsafe {
        if s.is_null() {
            return;
        }
        CString::from_raw(s)
    };
}

/**
 * Private functions
 */
fn cstr_to_string(s: *const c_char) -> String {
    if s.is_null() {
        String::new()
    } else {
        let c_str: &CStr = unsafe { CStr::from_ptr(s) };
        let slice: &str = c_str.to_str().unwrap();
        let t: String = slice.to_owned();
        t
    }
}