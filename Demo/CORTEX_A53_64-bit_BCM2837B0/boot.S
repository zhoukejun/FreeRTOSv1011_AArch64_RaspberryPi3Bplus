.global _boot

.global MMUTableL0

.global _vector_table


.set EL3_stack,         __el3_stack
.set EL2_stack,         __el2_stack
.set EL1_stack,         __el1_stack
.set EL0_stack,         __el0_stack

.set L0Table, MMUTableL0

.set vector_base,	_vector_table

.section .boot, "ax"
	b	_boot
	.text
	.type	_boot, %function
_boot:
	mov	x0, xzr
	mov	x1, xzr
	mov	x2, xzr
	mov	x3, xzr
	mov	x4, xzr
	mov	x5, xzr
	mov	x6, xzr
	mov	x7, xzr
	mov	x8, xzr
	mov	x9, xzr
	mov	x10, xzr
	mov	x11, xzr
	mov	x12, xzr
	mov	x13, xzr
	mov	x14, xzr
	mov	x15, xzr
	mov	x16, xzr
	mov	x17, xzr
	mov	x18, xzr
	mov	x19, xzr
	mov	x20, xzr
	mov	x21, xzr
	mov	x22, xzr
	mov	x23, xzr
	mov	x24, xzr
	mov	x25, xzr
	mov	x26, xzr
	mov	x27, xzr
	mov	x28, xzr
	mov	x29, xzr
	mov	x30, xzr

.if(1)
	#Read processor number, move slave processors to an infinite loop
	mrs	x7, MPIDR_EL1
	and	x7, x7, #3
	cbz	x7, OKToRun
EndlessLoop0:
	wfe
	b	EndlessLoop0
.endif
OKToRun:
	mrs	x0, currentEL
	cmp	x0, #0xC
	beq	InitEL3

	cmp	x0, #0x8
	beq	InitEL2

	cmp	x0, #0x4
	beq	InitEL1

	cmp	x0, #0x2
	beq	InitEL0

	b	error
InitEL3:
	/*Set vector table base address*/
	ldr	x1, =vector_base
	msr	VBAR_EL3, x1

	/* Define stack_pointer for current exception level */
	ldr	x2, =EL3_stack
	mov	sp, x2


	/* Configure SCR_EL3 */
	mov	w1, wzr
	orr	w1, w1, #(1 << 11)	//; Set ST bit (Secure EL1 can access CNTPS_TVAL_EL1, CNTPS_CTL_EL1 & CNTPS_CVAL_EL1)
	orr	w1, w1, #(1 << 10)	//; Set RW bit (EL1 is AArch64, as this is the secure world)
	orr	w1, w1, #(1 << 3)	//; Set EA bit (SError routed to EL3) 
	orr	w1, w1, #(1 << 2)	//; Set FIQ bit (FIQs routed to EL3) 
	orr	w1, w1, #(1 << 1)	//; Set IRQ bit (IRQs routed to EL3) 
	bic	x1, x1, #1
	msr	SCR_EL3, x1

	/* ARM Errata 855873 */
	mrs	x0, MIDR_EL1
	tst	x0, #(0xF << 20)
	b.ne	EnableHardwareCoherency
	
	mrs	x0, MIDR_EL1
	and	x0, x0, #0xF
	cmp	x0, #3
	b.lt	EnableHardwareCoherency

	mrs	x0, S3_1_C15_C2_0	//CPUACTLR_EL1	
	orr	x0, x0, #(1 << 44)
	msr	S3_1_C15_C2_0, x0
	isb

	/* Enable hardware coherency between cores */
EnableHardwareCoherency:
	mrs	x0, S3_1_C15_C2_1	//Read EL1 CPU Extended Control Register
	orr	x0, x0, #(1 << 6)	//Set the SMPEN bit
	msr	S3_1_C15_C2_1, x0	//Write EL1 CPU Extented Control Register
	isb

	/* Disable MMU first */
	mrs	x1, SCTLR_EL3		//Most of the SCTLR_EL3	bits are unknown at reset
	and	x1, x1, #~(1 << 0)
	msr	SCTLR_EL3, x1
	isb

	tlbi	ALLE3
	ic	IALLU	//Invalide I cache to PoU
	bl	invalidate_dcaches
	dsb	sy
	isb

	/* Enable SError Exception for asynchronous abort 
	 * Enable IRQ
	 * Enable FIQ
	 */
	mrs	x1, DAIF
	bic	x1, x1, #(1 << 8)
	bic	x1, x1, #(1 << 7)
	bic	x1, x1, #(1 << 6)
	msr	DAIF, x1
	isb

	/* Configure SCTLR_EL3 */
	mrs	x1, SCTLR_EL3		//Most of the SCTLR_EL3	bits are unknown at reset
	orr	x1, x1, #(1 << 12)	//Enable I cache
	orr	x1, x1, #(1 << 3)	//Enable SP alignment check
	orr	x1, x1, #(1 << 2)	//Enable caches
//	orr	x1, x1, #(1 << 0)	//Enable MMU
	msr	SCTLR_EL3, x1		
	dsb	sy
	isb

	b	clean_sbss_bss

InitEL2:
	/* Set vector table base address */
	ldr	x1, =vector_base
	msr	VBAR_EL2, x1

	/* Define stack pointer for current exception level */
	ldr	x2, =EL2_stack
	mov	sp, x2

	/* Enable FPU/SIMD */
	mov	x0, #0x33FF
	msr	cptr_el2, x0 
.if(0)
	/* ARM Errata 855873 */
	mrs	x0, MIDR_EL1
	tst	x0, #(0xF << 20)
	b.ne	EnableHardwareCoherencyEL2
	
	mrs	x0, MIDR_EL1
	and	x0, x0, #0xF
	cmp	x0, #3
	b.lt	EnableHardwareCoherencyEL2

	mrs	x0, S3_1_C15_C2_0	//CPUACTLR_EL1	
	orr	x0, x0, #(1 << 44)
	msr	S3_1_C15_C2_0, x0
	isb
.endif
	
	/* Enable hardware coherency between cores */
EnableHardwareCoherencyEL2:
		
	/* Disable MMU first */
	mov	x1, #0x0
	msr	SCTLR_EL2, x1
	isb

	tlbi	ALLE2

	ic	IALLU
	bl	invalidate_dcaches
	dsb	sy
	isb
	
	/* Enable SError Exception for asynchronous abort */
	mrs	x1, DAIF
	bic	x1, x1, #(0x1 << 8)
	msr	DAIF, x1

	/* Enable MMU */
	mov	x1, #0x0
	orr	x1, x1, #(1 << 18)	//Set WFE non trapping
	orr	x1, x1, #(1 << 17)	//Set WFI non trapping
	orr	x1, x1, #(1 << 12)	//Set I bit
	orr	x1, x1, #(1 << 5)	//Set CP15 barrier enable
	orr	x1, x1, #(1 << 2)	//Set C bit
	orr	x1, x1, #(1 << 0)	//Set M bit
	msr	SCTLR_EL2, x1
	isb

	b	clean_sbss_bss

InitEL1:
	b	error
	/* Set vector table base address */
	ldr	x1, =vector_base
	msr	VBAR_EL1, x1

	mrs	x0, CPACR_EL1
	bic	x0, x0, #(0x3 << 0x20)
	msr	CPACR_EL1, x0
	isb

	/* Define stack pointer for current exception level */
	ldr	x2, =EL1_stack
	mov	sp, x2

	/* Disable MMU first */
	mov	x1, #0x0
	msr	SCTLR_El1, x1
	isb

	TLBI	VMALLE1

	ic	IALLU
	bl	invalidate_dcaches
	dsb	sy
	isb
	
	/* Enable SError Exception for asynchronous abort */
	mrs	x1, DAIF
	bic	x1, x1, #(0x1 << 8)
	msr	DAIF, x1

	/* Enable MMU */
	mov	x1, #0x0
	orr	x1, x1, #(1 << 18)	//Set WFE non trapping
	orr	x1, x1, #(1 << 17)	//Set WFI non trapping
	orr	x1, x1, #(1 << 12)	//Set I bit
	orr	x1, x1, #(1 << 5)	//Set CP15 barrier enable
	orr	x1, x1, #(1 << 2)	//Set C bit
	orr	x1, x1, #(1 << 0)	//Set M bit
	msr	SCTLR_EL1, x1
	isb
	
	b	clean_sbss_bss

InitEL0:
	b	error

invalidate_dcaches:
	dmb	ISH
	mrs	x0, CLIDR_EL1		// x0 = CLIDR
	ubfx	w2, w0, #24, #3		// w2 = CLIDR.LoC
	cmp	w2, #0			// LoC is 0:
	b.eq	invalidateCaches_end	// No cleaning required and enable MMU
	mov	w1, #0			// w1 = level iterator

invalidateCaches_flush_level:
	add	w3, w1, w1, lsl #1	// w3 = w1 * 3 (right-shift for cache type)
	lsr	w3, w0, w3		// w3 = w0 >> w3
	ubfx	w3, w3, #0, #3		// w3 = cache type of this level
	cmp	w3, #2			// No cache at this level?
	b.lt	invalidateCaches_next_level

	lsl	w4, w1, #1
	msr	CSSELR_EL1, x4		// Select current cache level in CSSELR
	isb				// ISB required to reflect new CSIDR
	mrs	x4, CCSIDR_EL1		// w4 = CSIDR

	ubfx	w3, w4, #0, #3
	add	w3, w3, #2		// w3 = log2(line size)
	ubfx	w5, w4, #13, #15
	ubfx	w4, w4, #3, #10		// w4 = Way number
	clz	w6, w4			// w6 = 32 - log2(number of ways)

invalidateCaches_flush_set:
	mov	w8, w4			// w8 = Way number
invalidateCaches_flush_way:
	lsl	w7, w1, #1		// Fill level field
	lsl	w9, w5, w3
	orr	w7, w7, w9		// Fill index field
	lsl	w9, w8, w6
	orr	w7, w7, w9		// Fill way field
	dc	CISW, x7		// Invalidate by set/way to point of coherency
	subs	w8, w8, #1		// Decrement way
	b.ge	invalidateCaches_flush_way
	subs	w5, w5, #1		// Decrement set
	b.ge	invalidateCaches_flush_set

invalidateCaches_next_level:
	add	w1, w1, #1		// Next level
	cmp	w2, w1
	b.gt	invalidateCaches_flush_level

invalidateCaches_end:
	ret

clean_sbss_bss:
	ldr	w0, sbss_segment + 0
	ldr	w1, sbss_segment + 4
0:
	cbz	x1, 1f
	str	xzr, [x0], #8
	sub	x1, x1, #1
	cbnz	x1, 0b
1:

        ldr     w0, bss_segment + 0
        ldr     w1, bss_segment + 4
2:
        cbz     x1, 3f
        str     xzr, [x0], #8
        sub     x1, x1, #1
        cbnz    x1, 2b
3:
	
        ldr     w0, heap_segment + 0
        ldr     w1, heap_segment + 4
4:
        cbz     x1, 5f
        str     xzr, [x0], #8
        sub     x1, x1, #1
        cbnz    x1, 4b
5:
        bl      main

error:
	b	error

sbss_segment:
	.word	__sbss_start
	.word	__sbss_dwords


bss_segment:
	.word	__bss_start
	.word	__bss_dwords

heap_segment:
	.word	__heap_start
	.word	__heap_dwords


