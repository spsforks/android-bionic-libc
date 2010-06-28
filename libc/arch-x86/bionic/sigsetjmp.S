#include <machine/asm.h>
#include <machine/setjmp.h>

ENTRY(sigsetjmp)
	movl 4(%esp),%ecx
	movl 8(%esp),%eax
	movl %eax,28(%ecx)
	jz 1f
  PIC_PROLOGUE
	pushl $0
#ifdef PIC
	call PIC_PLT(_C_LABEL(sigblock))
#else
	call _C_LABEL(sigblock)
#endif
	addl $4,%esp
  PIC_EPILOGUE
	movl 4(%esp),%ecx
	movl %eax,24(%ecx)
  1:	movl 0(%esp),%edx
	movl %edx, 0(%ecx)
	movl %ebx, 4(%ecx)
	movl %esp, 8(%ecx)
	movl %ebp,12(%ecx)
	movl %esi,16(%ecx)
	movl %edi,20(%ecx)
	xorl %eax,%eax
	ret

ENTRY(siglongjmp)
	movl 4(%esp),%edx
	cmpl $0,28(%edx)
	jz 1f
  PIC_PROLOGUE
	pushl 24(%edx)
#ifdef PIC
	call PIC_PLT(_C_LABEL(sigsetmask))
#else
	call _C_LABEL(sigsetmask)
#endif
	addl $4,%esp
  PIC_EPILOGUE
  1:	movl 4(%esp),%edx
	movl 8(%esp),%eax
	movl 0(%edx),%ecx
	movl 4(%edx),%ebx
	movl 8(%edx),%esp
	movl 12(%edx),%ebp
	movl 16(%edx),%esi
	movl 20(%edx),%edi
	testl %eax,%eax
	jnz 2f
	incl %eax
  2:	movl %ecx,0(%esp)
	ret
