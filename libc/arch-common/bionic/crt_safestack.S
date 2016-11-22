	.section ".note.safestack", "a", %note
	.p2align 2
	.long .L1 - .L0
	.long .L3 - .L2
	.long 1
.L0:
	.asciz "SafeStack"  /* ELF_NOTE_SAFESTACK */
.L1:
	.p2align 2
.L2:
	.long 1  /* NT_SAFESTACK */
.L3:
	.p2align 2
