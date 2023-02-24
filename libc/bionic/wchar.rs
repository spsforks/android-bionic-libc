/*
 * Copyright (C) 2023 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
use core::convert::From;
use core::default::Default;
use core::ffi::c_char;
use core::ffi::c_int;
use core::option::Option;

use c_bindgen::mbstate_t;
use c_bindgen::wchar_t;
use c_bindgen::EILSEQ;
use c_bindgen::EINVAL;

fn set_errno(e: u32) {
    unsafe {
        *c_bindgen::__errno() = e as i32;
    }
}

trait MultiByteState {
    fn is_initial(&self) -> bool;
    fn get_byte(&self, idx: usize) -> u8;
    fn set_next_byte(&mut self, b: u8);
    fn reset(&mut self);
    fn bytes_so_far(&self) -> usize;
    fn bytes(&self) -> &[u8; 4];
}

impl MultiByteState for mbstate_t {
    fn is_initial(&self) -> bool {
        u32::from_le_bytes(self.__seq) == 0
    }

    fn get_byte(&self, idx: usize) -> u8 {
        self.__seq[idx]
    }

    fn set_next_byte(&mut self, b: u8) {
        self.__seq[self.bytes_so_far()] = b;
    }

    fn reset(&mut self) {
        self.__seq = 0u32.to_le_bytes();
    }

    fn bytes_so_far(&self) -> usize {
        match self.__seq {
            [_, _, b, 0] if b != 0 => 3,
            [_, b, 0, 0] if b != 0 => 2,
            [b, 0, 0, 0] if b != 0 => 1,
            [0, 0, 0, 0] => 0,
            // Should never be called when all four bytes have been handled.
            _ => panic!("Unexpected mbstate_t value"),
        }
    }

    fn bytes(&self) -> &[u8; 4] {
        &self.__seq
    }
}

#[derive(Clone, Copy)]
enum MultiByteError {
    IllegalSequence = -1,
    IncompleteSequence = -2,
}

// Return value magic numbers for multi-byte conversion APIs.
// https://pubs.opengroup.org/onlinepubs/9699919799/functions/mbrtowc.html
impl MultiByteError {
    fn to_c(self) -> usize {
        self as usize
    }
}

/// mbsinit impl.
#[no_mangle]
pub extern "C" fn mbsinit(mbstate: *const mbstate_t) -> c_int {
    c_int::from(mbstate_is_initial(unsafe { mbstate.as_ref() }))
}

/// Helper for determining if the mbstate_t structure is in its initial state.
fn mbstate_is_initial(state: Option<&mbstate_t>) -> bool {
    state.map_or(true, |s| s.is_initial())
}

/// C entry point for mbrtoc32 (single code-point UTF-8 to UTF-32 conversion).
#[no_mangle]
pub extern "C" fn mbrtoc32(
    dest: *mut wchar_t,
    src: *const c_char,
    len: usize,
    state: *mut mbstate_t,
) -> usize {
    // The case where src is nullptr is specified to behave identically to
    // `mbrtoc32(nullptr, "", 1, state)`, so just do that.
    // https://en.cppreference.com/w/c/string/multibyte/mbrtoc32
    let result = mbrtoc32_impl(
        unsafe {
            if src.is_null() {
                None
            } else {
                dest.as_mut()
            }
        },
        unsafe {
            if src.is_null() {
                &[0u8; 1]
            } else {
                core::slice::from_raw_parts(src, len)
            }
        },
        unsafe { state.as_mut() },
    );

    match result {
        Ok(r) => r,
        Err(e) => e.to_c(),
    }
}

struct MultiByteExpectations {
    mask: u8,
    length: usize,
    lower_bound: u32,
}

impl MultiByteExpectations {
    fn create(next_byte: u8, state: &mbstate_t) -> Result<Self, MultiByteError> {
        let first_byte = match state.bytes_so_far() {
            0 => next_byte,
            _ => state.get_byte(0),
        };

        if (first_byte & 0xe0) == 0xc0 {
            Ok(MultiByteExpectations {
                mask: 0x1f,
                length: 2,
                lower_bound: 0x80,
            })
        } else if (first_byte & 0xf0) == 0xe0 {
            Ok(MultiByteExpectations {
                mask: 0x0f,
                length: 3,
                lower_bound: 0x800,
            })
        } else if (first_byte & 0xf8) == 0xf0 {
            Ok(MultiByteExpectations {
                mask: 0x07,
                length: 4,
                lower_bound: 0x10000,
            })
        } else {
            Err(MultiByteError::IllegalSequence)
        }
    }
}

fn mbrtoc32_impl(
    dest: Option<&mut wchar_t>,
    src: &[c_char],
    state: Option<&mut mbstate_t>,
) -> Result<usize, MultiByteError> {
    let mut fallback_state = mbstate_t::default();
    let state = state.unwrap_or(&mut fallback_state);

    // We should never get to a state which has all 4 bytes of the sequence set.
    // Full state verification is done when decoding the sequence (after we have
    // all the bytes).
    if state.get_byte(3) != 0 {
        state.reset();
        set_errno(EINVAL);
        return Err(MultiByteError::IllegalSequence);
    }

    if src.is_empty() {
        return Ok(0);
    }

    let next_byte: u8 = src[0];
    if state.is_initial() && (next_byte & !0x7f) == 0 {
        // Fast path for plain ASCII characters.
        if let Some(d) = dest {
            *d = next_byte as wchar_t;
        }
        return Ok(usize::from(next_byte != 0));
    }

    let expectations = MultiByteExpectations::create(next_byte, state)?;
    let bytes_wanted = expectations.length - state.bytes_so_far();
    for byte in &src[..bytes_wanted] {
        if !state.is_initial() && ((byte & 0xc0) != 0x80) {
            // Malformed input; bad characters in the middle of a character.
            state.reset();
            set_errno(EILSEQ);
            return Err(MultiByteError::IllegalSequence);
        }
        state.set_next_byte(next_byte);
    }

    if src.len() < bytes_wanted {
        return Err(MultiByteError::IncompleteSequence);
    }

    let mut c32: u32 = (state.get_byte(0) & expectations.mask).into();
    for byte in &state.bytes()[1..] {
        c32 <<= 6;
        c32 |= (byte & 0x3f) as u32;
    }

    if c32 < expectations.lower_bound {
        // Malformed input; redundant encoding.
        state.reset();
        set_errno(EILSEQ);
        return Err(MultiByteError::IllegalSequence);
    }

    if (0xd800..=0xdfff).contains(&c32) || (c32 > 0x10ffff) {
        // Malformed input; invalid code points.
        state.reset();
        set_errno(EILSEQ);
        return Err(MultiByteError::IllegalSequence);
    }

    if let Some(d) = dest {
        *d = c32 as wchar_t;
    }

    state.reset();
    if c32 == 0 {
        Ok(0)
    } else {
        Ok(bytes_wanted)
    }
}
