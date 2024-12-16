use std::{char, ffi::{c_char, c_uchar, c_uint}};


#[no_mangle]
pub extern fn isAlpha(uint: c_uint) -> bool {
    match std::char::from_u32(uint) {
        Some(uchar) => return uchar.is_alphabetic(),
        None => return false
    };
}

#[no_mangle]
pub extern fn isDigit(uint: c_uint, radix: c_uchar) -> bool {
    match std::char::from_u32(uint) {
        Some(uchar) => return uchar.is_digit(radix as u32),
        None => return false
    };
}

#[no_mangle]
pub extern fn isHexDigit(uint: c_uint) -> bool {
    match std::char::from_u32(uint) {
        Some(uchar) => return uchar.is_digit(16),
        None => return false
    };
}

#[no_mangle]
pub extern fn isDecimalDigit(uint: c_uint) -> bool {
    match std::char::from_u32(uint) {
        Some(uchar) => return uchar.is_digit(10),
        None => return false
    };
}

#[no_mangle]
pub extern fn isOctalDigit(uint: c_uint) -> bool {
    match std::char::from_u32(uint) {
        Some(uchar) => return uchar.is_digit(8),
        None => return false
    };
}

#[no_mangle]
pub extern fn isBinaryDigit(uint: c_uint) -> bool {
    match std::char::from_u32(uint) {
        Some(uchar) => return uchar.is_digit(2),
        None => return false
    };
}

#[no_mangle]
pub extern fn isLowerCase(uint: c_uint) -> bool {
    match std::char::from_u32(uint) {
        Some(uchar) => return uchar.is_lowercase(),
        None => return false
    };
}

#[no_mangle]
pub extern fn isUpperCase(uint: c_uint) -> bool {
    match std::char::from_u32(uint) {
        Some(uchar) => return uchar.is_uppercase(),
        None => return false
    };
}

#[no_mangle]
pub extern fn isWhitespace(uint: c_uint) -> bool {
    match std::char::from_u32(uint) {
        Some(uchar) => return uchar.is_whitespace(),
        None => return false
    };
}

#[no_mangle]
pub extern fn isControlChar(uint: c_uint) -> bool {
    match std::char::from_u32(uint) {
        Some(uchar) => return uchar.is_control(),
        None => return false
    };
}