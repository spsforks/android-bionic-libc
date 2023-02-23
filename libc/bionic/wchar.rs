use core::ffi::c_int;
use core::option::Option;

use c_bindgen::mbstate_t;

/// mbsinit impl.
#[no_mangle]
pub extern "C" fn mbsinit(mbstate: *const mbstate_t) -> c_int {
    c_int::from(mbstate_is_initial(unsafe { mbstate.as_ref() }))
}

/// Helper for determining if the mbstate_t structure is in its initial state.
fn mbstate_is_initial(state: Option<&mbstate_t>) -> bool {
    state.map_or(true, |s| u32::from_le_bytes(s.__seq) == 0)
}
