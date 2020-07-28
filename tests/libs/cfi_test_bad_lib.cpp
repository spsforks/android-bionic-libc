// Mock an invalid CFI-enabled library.
__attribute__((aligned(4096))) extern "C" char temp[16] = {};
__asm__(".globl __cfi_check");
__asm__("__cfi_check = temp + 3"); // Not aligned to anything.
