#ifndef REGS_H_
#define REGS_H_

#include "types.h"

#define __arch_getl(a)		(*(volatile unsigned int*)(a))
#define __arch_putl(v,a)	(*(volatile unsigned int*)(a) = (v))

#define DMB		asm volatile ("dmb sy" : : : "memory")
#define dmb()		DMB
#define __iormb()	dmb()
#define __iowmb()	dmb()

#define readl(a)	({ unsigned int  __v = __arch_getl(a); __iormb(); __v; })
#define	writel(v,a)	({ unsigned int __v = v; __iowmb(); __arch_putl(__v,a); __v; })


#define PBASE 0x3F000000 //Physical Address for BCM2837

#define GPFSEL1  	(volatile uint32_t *)(PBASE+0x00200004)
#define GPPUD      	(volatile uint32_t *)(PBASE+0x00200094)
#define GPPUDCLK0  	(volatile uint32_t *)(PBASE+0x00200098)

#define AUX_ENABLES     (volatile uint32_t *)(PBASE+0x00215004)
#define AUX_MU_IO_REG   (volatile uint32_t *)(PBASE+0x00215040)
#define AUX_MU_IER_REG  (volatile uint32_t *)(PBASE+0x00215044)
#define AUX_MU_IIR_REG  (volatile uint32_t *)(PBASE+0x00215048)
#define AUX_MU_LCR_REG  (volatile uint32_t *)(PBASE+0x0021504C)
#define AUX_MU_MCR_REG  (volatile uint32_t *)(PBASE+0x00215050)
#define AUX_MU_LSR_REG  (volatile uint32_t *)(PBASE+0x00215054)
#define AUX_MU_MSR_REG  (volatile uint32_t *)(PBASE+0x00215058)
#define AUX_MU_SCRATCH  (volatile uint32_t *)(PBASE+0x0021505C)
#define AUX_MU_CNTL_REG (volatile uint32_t *)(PBASE+0x00215060)
#define AUX_MU_STAT_REG (volatile uint32_t *)(PBASE+0x00215064)
#define AUX_MU_BAUD_REG (volatile uint32_t *)(PBASE+0x00215068)

/* Interrupt Control Regs */
#define INTC_BASE	(PBASE + 0x0000B000)

#define IRQ_BASIC_PENDING	(volatile uint32_t *)(INTC_BASE + 0x00000200)
#define IRQ_PENDING_1		(volatile uint32_t *)(INTC_BASE + 0x00000204)
#define IRQ_PENDING_2		(volatile uint32_t *)(INTC_BASE + 0x00000208)
#define FIQ_CONTROL		(volatile uint32_t *)(INTC_BASE + 0x0000020C)
#define ENABLE_IRQ_1		(volatile uint32_t *)(INTC_BASE + 0x00000210)
#define ENABLE_IRQ_2		(volatile uint32_t *)(INTC_BASE + 0x00000214)
#define ENABLE_BASIC_IRQ	(volatile uint32_t *)(INTC_BASE + 0x00000218)
#define DISABLE_IRQ_1		(volatile uint32_t *)(INTC_BASE + 0x0000021C)
#define DISABLE_IRQ_2		(volatile uint32_t *)(INTC_BASE + 0x00000220)
#define DISABLE_BASIC_IRQ	(volatile uint32_t *)(INTC_BASE + 0x00000224)


/* System Timer Regs */
#define SYS_TIME_BASE	(PBASE + 0x00003000)

#define SYS_TIMER_CS	(volatile uint32_t *)(SYS_TIME_BASE + 0x00000000)
#define SYS_TIMER_CLO	(volatile uint32_t *)(SYS_TIME_BASE + 0x00000004)
#define SYS_TIMER_CHI	(volatile uint32_t *)(SYS_TIME_BASE + 0x00000008)
#define SYS_TIMER_C_0	(volatile uint32_t *)(SYS_TIME_BASE + 0x0000000C)
#define SYS_TIMER_C_1	(volatile uint32_t *)(SYS_TIME_BASE + 0x00000010)
#define SYS_TIMER_C_2	(volatile uint32_t *)(SYS_TIME_BASE + 0x00000014)
#define SYS_TIMER_C_3	(volatile uint32_t *)(SYS_TIME_BASE + 0x00000018)


/* ARM Timer Regs */
#define ARM_TIME_BASE	(PBASE + 0x0000B000)

#define ARM_TIMER_LOAD		(volatile uint32_t *)(ARM_TIME_BASE + 0x00000400)
#define ARM_TIMER_VALUE		(volatile uint32_t *)(ARM_TIME_BASE + 0x00000404)
#define ARM_TIMER_CTRL		(volatile uint32_t *)(ARM_TIME_BASE + 0x00000408)
#define ARM_TIMER_CLEAR_ACK	(volatile uint32_t *)(ARM_TIME_BASE + 0x0000040C)
#define ARM_TIMER_RAW_IRQ	(volatile uint32_t *)(ARM_TIME_BASE + 0x00000410)
#define ARM_TIMER_MASKED_IRQ	(volatile uint32_t *)(ARM_TIME_BASE + 0x00000414)
#define ARM_TIMER_RELOAD	(volatile uint32_t *)(ARM_TIME_BASE + 0x00000418)
#define ARM_TIMER_PRE_DIV	(volatile uint32_t *)(ARM_TIME_BASE + 0x0000041C)
#define ARM_TIMER_FREE_R_CNT	(volatile uint32_t *)(ARM_TIME_BASE + 0x00000420)


static inline void iowrite32 (uint32_t value, volatile uint32_t *addr)
{
	writel(value, addr);
}

static inline uint32_t ioread32 (const volatile uint32_t *addr)
{
	return readl(addr);
}

#endif /* REGS_H_ */
