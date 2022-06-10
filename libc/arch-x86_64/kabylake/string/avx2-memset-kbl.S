#include <private/bionic_asm.h>
#include "cache.h" 

#ifndef ALIGN
# define ALIGN(n)	.p2align n
#endif

#define MEMSET memset_avx2

# define BRANCH_TO_JMPTBL_ENTRY(TABLE)				\
    /* Get the address of the jump table.  */			\
    leaq	.L_Size_Jmp_Table(%rip), %r10;   \
    /* Get the entry and convert the relative offset to the	\
       absolute address.  */					\
    movslq	(%r10,%rdx,4), %r11; \
    add	%r10, %r11;  \
    /* We loaded the jump table and adjusted RAX. Go.  */ \
    jmp	*%r11 \

    .section        .text.avx2,"ax",@progbits
ENTRY(MEMSET)
   # %rdi = dst, %rsi = byte, %rdx = size, %rcx = 64bit val, %ymm0 = 256bit val, %xmm0 = 128bit val
   movq         %rdi, %rax
   and	        $0xff, %rsi
   mov          $0x0101010101010101, %rcx
   imul         %rsi, %rcx
   vmovd        %rcx, %xmm1 
   vpbroadcastb %xmm1, %ymm0
   cmpq         $256, %rdx
   ja          .Lpadding
   BRANCH_TO_JMPTBL_ENTRY(.L_Size_Jmp_Table)

ALIGN(4)
.Lpadding:
   movq     %rdi, %r11
   movq     $32, %r10
   andq     $31, %r11
   subq     %r11, %r10
   andq     $31, %r10
   vmovdqu  %ymm0, (%rdi)
   addq     %r10, %rdi
   subq     %r10, %rdx
   cmpq     $256, %rdx
   jae      .L_256_or_more_normal
   BRANCH_TO_JMPTBL_ENTRY(.L_Size_Jmp_Table)

.L_256_or_more:
#ifdef SHARED_CACHE_SIZE
   cmp	$SHARED_CACHE_SIZE, %rdx
#else
   cmp	__x86_64_shared_cache_size(%rip), %rdx
#endif
   ja	.L_256_or_more_nt

ALIGN(4)
.L_256_or_more_normal:
   vmovdqa %ymm0, (%rdi)
   vmovdqa %ymm0, 32(%rdi)
   vmovdqa %ymm0, 64(%rdi)
   vmovdqa %ymm0, 96(%rdi)
   vmovdqa %ymm0, 128(%rdi)
   vmovdqa %ymm0, 160(%rdi)
   vmovdqa %ymm0, 192(%rdi)
   vmovdqa %ymm0, 224(%rdi)
   subq $256, %rdx
   addq $256, %rdi
   cmp $256, %rdx
   ja .L_256_or_more_normal
   BRANCH_TO_JMPTBL_ENTRY(.L_Size_Jmp_Table)

ALIGN(4)
.L_256_or_more_nt:
   movntdq  %xmm0, (%rdi)
   movntdq  %xmm0, 16(%rdi)
   movntdq  %xmm0, 32(%rdi)
   movntdq  %xmm0, 48(%rdi)
   movntdq  %xmm0, 64(%rdi)
   movntdq  %xmm0, 80(%rdi)
   movntdq  %xmm0, 96(%rdi)
   movntdq  %xmm0, 112(%rdi)
   movntdq  %xmm0, 128(%rdi)
   movntdq  %xmm0, 144(%rdi)
   movntdq  %xmm0, 160(%rdi)
   movntdq  %xmm0, 176(%rdi)
   movntdq  %xmm0, 192(%rdi)
   movntdq  %xmm0, 208(%rdi)
   movntdq  %xmm0, 224(%rdi)
   movntdq  %xmm0, 240(%rdi)
   subq     $256, %rdx
   addq     $256, %rdi
   cmp      $256, %rdx
   ja .L_256_or_more_nt
   sfence
   BRANCH_TO_JMPTBL_ENTRY(.L_Size_Jmp_Table)

ALIGN(4)
.L_Size_225_256:
    vmovdqu %ymm0, (%rdi)
    vmovdqu %ymm0, 32(%rdi)
    vmovdqu %ymm0, 64(%rdi)
    vmovdqu %ymm0, 96(%rdi)
    vmovdqu %ymm0, -128(%rdi, %rdx)
    vmovdqu %ymm0, -96(%rdi, %rdx)
    vmovdqu %ymm0, -64(%rdi, %rdx)
    vmovdqu %ymm0, -32(%rdi, %rdx)
    retq
.L_Size_193_224:
    vmovdqu %ymm0, (%rdi)
    vmovdqu %ymm0, 32(%rdi)
    vmovdqu %ymm0, 64(%rdi)
    vmovdqu %ymm0, 96(%rdi)
    vmovdqu %ymm0, -96(%rdi, %rdx)
    vmovdqu %ymm0, -64(%rdi, %rdx)
    vmovdqu %ymm0, -32(%rdi, %rdx)
    retq
.L_Size_161_192:
    vmovdqu %ymm0, (%rdi)
    vmovdqu %ymm0, 32(%rdi)
    vmovdqu %ymm0, 64(%rdi)
    vmovdqu %ymm0, 96(%rdi)
    vmovdqu %ymm0, -64(%rdi, %rdx)
    vmovdqu %ymm0, -32(%rdi, %rdx)
    retq
.L_Size_129_160:
    vmovdqu %ymm0, (%rdi)
    vmovdqu %ymm0, 32(%rdi)
    vmovdqu %ymm0, 64(%rdi)
    vmovdqu %ymm0, 96(%rdi)
    vmovdqu %ymm0, -32(%rdi, %rdx)
    retq
.L_Size_128:
    vmovdqu %ymm0, (%rdi)
    vmovdqu %ymm0, 32(%rdi)
    vmovdqu %ymm0, 64(%rdi)
    vmovdqu %ymm0, 96(%rdi)
    retq

ALIGN(4)
.L_Size_93_127:
    vmovdqu %ymm0, (%rdi)
    vmovdqu %ymm0, 32(%rdi)
    vmovdqu %ymm0, -64(%rdi, %rdx)
    vmovdqu %ymm0, -32(%rdi, %rdx)
    retq
.L_Size_65_92:
    vmovdqu %ymm0, (%rdi)
    vmovdqu %ymm0, 32(%rdi)
    vmovdqu %ymm0, -32(%rdi, %rdx)
    retq
.L_Size_64:
    vmovdqu %ymm0, (%rdi)
    vmovdqu %ymm0, 32(%rdi)
    retq

ALIGN(4)
.L_Size_33_63:
    vmovdqu %ymm0, (%rdi)
    vmovdqu %ymm0, -32(%rdi, %rdx)
    retq
.L_Size_32:
    vmovdqu %ymm0, (%rdi)
    retq

ALIGN(4)
.L_Size_17_31:
    vmovdqu %xmm0, (%rdi)
    vmovdqu %xmm0, -16(%rdi, %rdx)
    retq
.L_Size_16:
    vmovdqu %xmm0, (%rdi)
    retq
.L_Size_9_15:
    movq %rcx, -8(%rdi, %rdx)
.L_Size_8:
    movq %rcx, (%rdi)
    retq
.L_Size_5_7:
    movl %ecx, -4(%rdi, %rdx)
.L_Size_4:
    movl %ecx, (%rdi)
    retq
.L_Size_3:
    movw %cx, -2(%rdi, %rdx)
.L_Size_2:
    movw %cx, (%rdi)
    retq
.L_Size_1:
    movb %cl, (%rdi)
    retq
.L_Size_0:
    retq
END(MEMSET)

    .section        .rodata,"a",@progbits
    .p2align        2 
.L_Size_Jmp_Table:
    .long   .L_Size_0 - .L_Size_Jmp_Table
    .long   .L_Size_1 - .L_Size_Jmp_Table
    .long   .L_Size_2 - .L_Size_Jmp_Table
    .long   .L_Size_3 - .L_Size_Jmp_Table
    .long   .L_Size_4 - .L_Size_Jmp_Table
    .long   .L_Size_5_7 - .L_Size_Jmp_Table
    .long   .L_Size_5_7 - .L_Size_Jmp_Table
    .long   .L_Size_5_7 - .L_Size_Jmp_Table
    .long   .L_Size_8 - .L_Size_Jmp_Table
    .long   .L_Size_9_15 - .L_Size_Jmp_Table
    .long   .L_Size_9_15 - .L_Size_Jmp_Table
    .long   .L_Size_9_15 - .L_Size_Jmp_Table
    .long   .L_Size_9_15 - .L_Size_Jmp_Table
    .long   .L_Size_9_15 - .L_Size_Jmp_Table
    .long   .L_Size_9_15 - .L_Size_Jmp_Table
    .long   .L_Size_9_15 - .L_Size_Jmp_Table
    .long   .L_Size_16 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_17_31 - .L_Size_Jmp_Table
    .long   .L_Size_32 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_33_63 - .L_Size_Jmp_Table
    .long   .L_Size_64 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_65_92 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_93_127 - .L_Size_Jmp_Table
    .long   .L_Size_128 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_129_160 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_161_192 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_193_224 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table
    .long   .L_Size_225_256 - .L_Size_Jmp_Table

