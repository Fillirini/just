use std::ffi::c_uint;

pub mod predicate;

#[no_mangle]
// workaround, ignoring the possibility of a multi-character result
pub extern fn toUpperCase(uint: c_uint) -> c_uint {
    match std::char::from_u32(uint) {
        Some(uchar) => {
            match uchar.to_uppercase().next() {
                Some(c) => return c as u32,
                None => return 0
            }
        }
        None => return 0
    };
}


#[no_mangle]
// workaround, ignoring the possibility of a multi-character result
pub extern fn toLowerCase(uint: c_uint) -> c_uint {
    match std::char::from_u32(uint) {
        Some(uchar) => {
            match uchar.to_lowercase().next() {
                Some(c) => return c as u32,
                None => return 0
            }
        }
        None => return 0
    };
}