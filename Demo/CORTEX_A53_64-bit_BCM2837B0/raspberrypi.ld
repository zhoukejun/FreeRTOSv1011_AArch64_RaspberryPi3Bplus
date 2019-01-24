_STACK_SIZE = DEFINED(_STACK_SIZE) ? _STACK_SIZE : 0x20000;
_HEAP_SIZE = DEFINED(_HEAP_SIZE) ? _HEAP_SIZE : 0x20000;

_EL0_STACK_SIZE = DEFINED(_EL0_STACK_SIZE) ? _EL0_STACK_SIZE : 0x20000;
_EL1_STACK_SIZE = DEFINED(_EL1_STACK_SIZE) ? _EL1_STACK_SIZE : 0x20000;
_EL2_STACK_SIZE = DEFINED(_EL2_STACK_SIZE) ? _EL2_STACK_SIZE : 0x20000;
_EL3_STACK_SIZE = DEFINED(_EL3_STACK_SIZE) ? _EL3_STACK_SIZE : 0x20000;

MEMORY
{
	VECTORS	(rwx) : ORIGIN = 0x00080000, LENGTH =32M
	SRAM (rwx) : ORIGIN = 0x00000000, LENGTH = 32M
}

SECTIONS
{


	.text :{
		KEEP(*(.boot))
		. = 0x1000;
		. = ALIGN(64);
		*(.text)
		*(.text.*)
		*(.gnu.linkonce.t.*)
		*(.plt)
		*(.gnu_warning)
		*(.gcc_execpt_table)
		*(.glue_7)
		*(.glue_7t)
		*(.ARM.extab)
		*(.gnu.linkonce.armextab.*)
	} > SRAM

	.vectors :{
		. = ALIGN(64);
		KEEP(*(.vector_table))
	} > VECTORS


	.rodata : {
		*(.rodata)
		*(.rodata.*)
		*(.gnu.linkonce.r*)
	}

 	.data : {
	       	*(.data)
		*(.data.*)
		*(.gnu.linkonce.d*)
	}

	.sbss (NOLOAD) : {
		. = ALIGN(64);
		__sbss_start = .;
		*(.sbss)
		*(.sbss.*)
		*(.gnu.linkonce.sb.*)
		. = ALIGN(64);
		__sbss_end = .;
		__sbss_dwords = (__sbss_end - __sbss_start) >> 3;
	}

	.bss (NOLOAD): {
		__bss_start = ALIGN(64);
		*(.bss)
		*(.bss.*)
		*(COMMON)
		*(.gnu.linkonce.b.*)
		__bss_end = ALIGN(64);
		__bss_dwords = (__bss_end - __bss_start) >> 3;
	}

	.heap (NOLOAD) : {
		. = ALIGN(64);
		_heap = .;
		HeapBase = .;
		__heap_start = .;
		. += _HEAP_SIZE;
		. = ALIGN(64);
		__heap_end = .;
		__heap_dwords = (__heap_end - __heap_start) >> 3;
		HeapLimit = .;
	}

	.stack (NOLOAD) : {
		. = ALIGN(64);
		_el3_stack_end = .;
		. += _EL3_STACK_SIZE;
		. = ALIGN(64);
		__el3_stack = .;
		_el2_stack_end = .;
		. += _EL2_STACK_SIZE;
		. = ALIGN(64);
		__el2_stack = .;
		_el1_stack_end = .;
		. += _EL1_STACK_SIZE;
		. = ALIGN(64);
		__el1_stack = .;
		_el0_stack_end = .;
		. += _EL0_STACK_SIZE;
		. = ALIGN(64);
		__el0_stack = .;
	}

_end = .;
}
