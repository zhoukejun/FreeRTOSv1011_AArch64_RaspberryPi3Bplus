/**
 *	ARM TIMER
 *
 *	
 **/
#include "regs.h"

#define configCPU_CLOCK_HZ                      ( ( unsigned long ) 240000000 )
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )

void bcm_arm_timer_init(void)
{

	iowrite32(0x0, ARM_TIMER_CTRL);
}

void bcm_arm_timer_pre_scaler_set(uint32_t pre_scaler)
{
	/* 
	 * 32bit Time Control Register
	 * Bit(s) 23:16	pre-scaler
	 * Bit(s) 9	Free Running Counter Enabled
	 * Bit(s) 8	Timer halted if ARM is in debug halted mode
	 * Bit(s) 7	Timer enabled
	 * Bit(s) 5	Timer interrupt enabled
	 * Bit(s) 1	0:16-bit counter; 1:32-bit counter
	 * Bit(s) 0	Not used
	 */
	iowrite32((pre_scaler << 16) | ioread32(ARM_TIMER_CTRL), ARM_TIMER_CTRL);
	
}

void bcm_arm_timer_free_running_counter_set(uint32_t fr_cnt)
{
	iowrite32(fr_cnt, ARM_TIMER_LOAD);
}

void bcm_arm_timer_start_running()
{
	iowrite32(((0x1 << 9) | (0x1 << 7) | (0x1 << 5)) | ioread32(ARM_TIMER_CTRL), ARM_TIMER_CTRL);
}

void bcm_arm_timer_clear_irq()
{
	iowrite32(0, ARM_TIMER_CLEAR_ACK);
}

void bcm_arm_timer_check_raw_irq()
{
	print("bcm_arm_timer_check_raw_irq()");
	print("read from ARM_TIMER_RAW_IRQ is :");
	hexstring(ioread32(ARM_TIMER_RAW_IRQ));
}

void bcm_arm_timer_check_masked_irq()
{
	print("bcm_arm_timer_check_masked_irq()");
	print("read from ARM_TIMER_MASKED_IRQ is :");
	hexstring(ioread32(ARM_TIMER_MASKED_IRQ));
}
