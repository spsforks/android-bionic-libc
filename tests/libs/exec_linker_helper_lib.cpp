// Verify that the linker can find exec_linker_helper_lib.so using the
// executable's $ORIGIN runpath, even when the executable is inside a zip file.

const char* helper_func() {
  return "helper_func called";
}
