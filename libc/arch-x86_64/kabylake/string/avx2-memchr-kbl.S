#ifndef L
# define L(label)	.L##label
#endif

#ifndef cfi_startproc
# define cfi_startproc	.cfi_startproc
#endif

#ifndef cfi_endproc
# define cfi_endproc	.cfi_endproc
#endif

#ifndef cfi_rel_offset
# define cfi_rel_offset(reg, off)	.cfi_rel_offset reg, off
#endif

#ifndef cfi_restore
# define cfi_restore(reg)	.cfi_restore reg
#endif

#ifndef cfi_adjust_cfa_offset
# define cfi_adjust_cfa_offset(off)	.cfi_adjust_cfa_offset off
#endif

#ifndef ENTRY
# define ENTRY(name)		\
	.type name,  @function;		\
	.globl name;		\
	.p2align 4;		\
name:		\
	cfi_startproc
#endif

#ifndef END
# define END(name)		\
	cfi_endproc;		\
	.size name, .-name
#endif

#define CFI_PUSH(REG)		\
	cfi_adjust_cfa_offset (4);		\
	cfi_rel_offset (REG, 0)

#define CFI_POP(REG)		\
	cfi_adjust_cfa_offset (-4);		\
	cfi_restore (REG)

#define PUSH(REG)	push REG;
#define POP(REG)	pop REG;

#define ENTRANCE	PUSH (%rbx);
#define RETURN_END	POP (%rbx); ret
#define RETURN		RETURN_END;

# ifndef MEMCHR
#  define MEMCHR          memchr_avx2
# endif

# ifdef USE_AS_WMEMCHR
#  define VPCMPEQ         vpcmpeqd
# else
#  define VPCMPEQ         vpcmpeqb
# endif

# ifndef VZEROUPPER
#  define VZEROUPPER  vzeroupper
# endif

# define VEC_SIZE 32

                .section .text.avx,"ax",@progbits
ENTRY (MEMCHR)
# ifndef USE_AS_RAWMEMCHR
                /* Check for zero length.  */
                testq      %rdx, %rdx
                jz             L(null)
# endif
                movl      %edi, %ecx
                /* Broadcast CHAR to YMM0.  */
                vmovd  %esi, %xmm0
# ifdef USE_AS_WMEMCHR
                shl          $2, %rdx
                vpbroadcastd %xmm0, %ymm0
# else
                vpbroadcastb %xmm0, %ymm0
# endif
                /* Check if we may cross page boundary with one vector load.  */
                andl       $(2 * VEC_SIZE - 1), %ecx
                cmpl      $VEC_SIZE, %ecx
                ja             L(cros_page_boundary)

                /* Check the first VEC_SIZE bytes.  */
                VPCMPEQ (%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax

# ifndef USE_AS_RAWMEMCHR
                jnz          L(first_vec_x0_check)
                /* Adjust length and check the end of data.  */
                subq      $VEC_SIZE, %rdx
                jbe          L(zero)
# else
                jnz          L(first_vec_x0)
# endif

                /* Align data for aligned loads in the loop.  */
                addq      $VEC_SIZE, %rdi
                andl       $(VEC_SIZE - 1), %ecx
                andq      $-VEC_SIZE, %rdi

# ifndef USE_AS_RAWMEMCHR
                /* Adjust length.  */
                addq      %rcx, %rdx

                subq      $(VEC_SIZE * 4), %rdx
                jbe          L(last_4x_vec_or_less)
# endif
                jmp         L(more_4x_vec)

                .p2align 4
L(cros_page_boundary):
                andl       $(VEC_SIZE - 1), %ecx
                andq      $-VEC_SIZE, %rdi
                VPCMPEQ (%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                /* Remove the leading bytes.  */
                sarl         %cl, %eax
                testl       %eax, %eax
                jz             L(aligned_more)
                tzcntl     %eax, %eax
# ifndef USE_AS_RAWMEMCHR
                /* Check the end of data.  */
                cmpq     %rax, %rdx
                jbe          L(zero)
# endif
                addq      %rdi, %rax
                addq      %rcx, %rax
                VZEROUPPER
                ret

                .p2align 4
L(aligned_more):
# ifndef USE_AS_RAWMEMCHR
        /* Calculate "rdx + rcx - VEC_SIZE" with "rdx - (VEC_SIZE - rcx)"
                  instead of "(rdx + rcx) - VEC_SIZE" to void possible addition
                   overflow.  */
                negq      %rcx
                addq      $VEC_SIZE, %rcx

                /* Check the end of data.  */
                subq      %rcx, %rdx
                jbe          L(zero)
# endif

                addq      $VEC_SIZE, %rdi

# ifndef USE_AS_RAWMEMCHR
                subq      $(VEC_SIZE * 4), %rdx
                jbe          L(last_4x_vec_or_less)
# endif

L(more_4x_vec):
                /* Check the first 4 * VEC_SIZE.  Only one VEC_SIZE at a time
                   since data is only aligned to VEC_SIZE.  */
                VPCMPEQ (%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax
                jnz          L(first_vec_x0)

                VPCMPEQ VEC_SIZE(%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax
                jnz          L(first_vec_x1)

                VPCMPEQ (VEC_SIZE * 2)(%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax
                jnz          L(first_vec_x2)

                VPCMPEQ (VEC_SIZE * 3)(%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax
                jnz          L(first_vec_x3)

                addq      $(VEC_SIZE * 4), %rdi

# ifndef USE_AS_RAWMEMCHR
                subq      $(VEC_SIZE * 4), %rdx
                jbe          L(last_4x_vec_or_less)
# endif

                /* Align data to 4 * VEC_SIZE.  */
                movq     %rdi, %rcx
                andl       $(4 * VEC_SIZE - 1), %ecx
                andq      $-(4 * VEC_SIZE), %rdi

# ifndef USE_AS_RAWMEMCHR
                /* Adjust length.  */
                addq      %rcx, %rdx
# endif

                .p2align 4
L(loop_4x_vec):
                /* Compare 4 * VEC at a time forward.  */
                VPCMPEQ (%rdi), %ymm0, %ymm1
                VPCMPEQ VEC_SIZE(%rdi), %ymm0, %ymm2
                VPCMPEQ (VEC_SIZE * 2)(%rdi), %ymm0, %ymm3
                VPCMPEQ (VEC_SIZE * 3)(%rdi), %ymm0, %ymm4

                vpor       %ymm1, %ymm2, %ymm5
                vpor       %ymm3, %ymm4, %ymm6
                vpor       %ymm5, %ymm6, %ymm5

                vpmovmskb %ymm5, %eax
                testl       %eax, %eax
                jnz          L(4x_vec_end)

                addq      $(VEC_SIZE * 4), %rdi

# ifdef USE_AS_RAWMEMCHR
                jmp         L(loop_4x_vec)
# else
                subq      $(VEC_SIZE * 4), %rdx
                ja             L(loop_4x_vec)

L(last_4x_vec_or_less):
                /* Less than 4 * VEC and aligned to VEC_SIZE.  */
                addl       $(VEC_SIZE * 2), %edx
                jle           L(last_2x_vec)

                VPCMPEQ (%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax
                jnz          L(first_vec_x0)

                VPCMPEQ VEC_SIZE(%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax
                jnz          L(first_vec_x1)

                VPCMPEQ (VEC_SIZE * 2)(%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax

                jnz          L(first_vec_x2_check)
                subl       $VEC_SIZE, %edx
                jle           L(zero)

                VPCMPEQ (VEC_SIZE * 3)(%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax

                jnz          L(first_vec_x3_check)
                xorl        %eax, %eax
                VZEROUPPER
                ret

                .p2align 4
L(last_2x_vec):
                addl       $(VEC_SIZE * 2), %edx
                VPCMPEQ (%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax

                jnz          L(first_vec_x0_check)
                subl       $VEC_SIZE, %edx
                jle           L(zero)

                VPCMPEQ VEC_SIZE(%rdi), %ymm0, %ymm1
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax
                jnz          L(first_vec_x1_check)
                xorl        %eax, %eax
                VZEROUPPER
                ret

                .p2align 4
L(first_vec_x0_check):
                tzcntl     %eax, %eax
                /* Check the end of data.  */
                cmpq     %rax, %rdx
                jbe          L(zero)
                addq      %rdi, %rax
                VZEROUPPER
                ret

                .p2align 4
L(first_vec_x1_check):
                tzcntl     %eax, %eax
                /* Check the end of data.  */
                cmpq     %rax, %rdx
                jbe          L(zero)
                addq      $VEC_SIZE, %rax
                addq      %rdi, %rax
                VZEROUPPER
                ret

                .p2align 4
L(first_vec_x2_check):
                tzcntl     %eax, %eax
                /* Check the end of data.  */
                cmpq     %rax, %rdx
                jbe          L(zero)
                addq      $(VEC_SIZE * 2), %rax
                addq      %rdi, %rax
                VZEROUPPER
                ret

                .p2align 4
L(first_vec_x3_check):
                tzcntl     %eax, %eax
                /* Check the end of data.  */
                cmpq     %rax, %rdx
                jbe          L(zero)
                addq      $(VEC_SIZE * 3), %rax
                addq      %rdi, %rax
                VZEROUPPER
                ret

                .p2align 4
L(zero):
                VZEROUPPER
L(null):
                xorl        %eax, %eax
                ret
# endif

                .p2align 4
L(first_vec_x0):
                tzcntl     %eax, %eax
                addq      %rdi, %rax
                VZEROUPPER
                ret

                .p2align 4
L(first_vec_x1):
                tzcntl     %eax, %eax
                addq      $VEC_SIZE, %rax
                addq      %rdi, %rax
                VZEROUPPER
                ret

                .p2align 4
L(first_vec_x2):
                tzcntl     %eax, %eax
                addq      $(VEC_SIZE * 2), %rax
                addq      %rdi, %rax
                VZEROUPPER
                ret

                .p2align 4
L(4x_vec_end):
                vpmovmskb %ymm1, %eax
                testl       %eax, %eax
                jnz          L(first_vec_x0)
                vpmovmskb %ymm2, %eax
                testl       %eax, %eax
                jnz          L(first_vec_x1)
                vpmovmskb %ymm3, %eax
                testl       %eax, %eax
                jnz          L(first_vec_x2)
                vpmovmskb %ymm4, %eax
                testl       %eax, %eax
L(first_vec_x3):
                tzcntl     %eax, %eax
                addq      $(VEC_SIZE * 3), %rax
                addq      %rdi, %rax
                VZEROUPPER
                ret

END (MEMCHR)
