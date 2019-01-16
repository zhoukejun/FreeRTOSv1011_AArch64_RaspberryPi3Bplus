.global	_startup

.Lsbss_start:
	.quad	__sbss_start

.Lsbss_end:
	.quad	__sbss_end

.Lbss_start:
	.quad	__bss_start

.Lbss_end:
	.quad	__bss_end


.section .crt0, "ax"

_startup:

	/* clear sbss */
	ldr 	x1, .Lsbss_start		/* calculate beginning of the SBSS */
	ldr	x2, .Lsbss_end		/* calculate end of the SBSS */

.Lloop_sbss:
	cmp	x1, x2
	bge	.Lenclsbss		/* If no SBSS, no clearing required */
	str	xzr, [x1], #8
	b	.Lloop_sbss

.Lenclsbss:
	/* clear bss */
	ldr	x1,.Lbss_start		/* calculate beginning of the BSS */
	ldr	x2,.Lbss_end		/* calculate end of the BSS */

.Lloop_bss:
	cmp	x1,x2
	bge	.Lenclbss		/* If no BSS, no clearing required */
	str	xzr, [x1], #8
	b	.Lloop_bss

.Lenclbss:
	/* run global constructors */
//	bl __libc_init_array

	/* make sure argc and argv are valid */
	mov	x0, #0
	mov	x1, #0

	bl	main			/* Jump to main C code */

	/* Cleanup global constructors */
//	bl __libc_fini_array

.Lexit:	/* should never get here */
	b .Lexit

.Lstart:
	.size	_startup, .Lstart - _startup
