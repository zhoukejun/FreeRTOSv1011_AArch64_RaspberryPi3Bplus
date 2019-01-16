/* Scheduler includes. */
#include "FreeRTOS.h"
#include "regs.h"
#include "task.h"
#include "irq.h"
#include "arm_timer.h"
#include "portmacro.h"




/*
 * The application must provide a function that configures a peripheral to
 * create the FreeRTOS tick interrupt, then define configSETUP_TICK_INTERRUPT()
 * in FreeRTOSConfig.h to call the function.
 */

void vSetupTickInterrupt(void)
{
	 /* Interrupts are disabled when this function is called. */

	/*
	 * Use sys_clk as the clock source, no from the apb_clk.
	 * As the sys_clk will change base on the system performance changing.
	 * Using the sys_clk as the Operating System tick timer, OS scheduler performance will change.
	 *
	 * Free Running Counter Frequency = sys_clk / (pre-scaler + 1)
	 * As we would like setting the OS tick to 1ms, so the OS tick interrupt Frequency is 1000 Hz
	 * sys_clk = configCPU_CLOCK_HZ = 240 MHz;
	 * os_tick = configTICK_RATE_HZ = 1000 Hz
	 *
	 * !!!REMINDER!!!: The OS tick Frequency is not equal to the Free Running Counter Frequency.
	 * And more, the pre-scaler is only 8 bit (255 in max), sys_clk is 240 MHz (240000000).
	 * Free Running Counter Frequency is 1 MHz (100000 Hz) in minimum.
	 * OS tick Frequency = Free Running Counter Frequency / (Count Down Load Value + 1) .
	 * 
	 * pre-scaler = 240 - 1 = 0xEF;
	 * Count Down Load Value = 1000 - 1;
	 */
	bcm_arm_timer_init();
	bcm_arm_timer_pre_scaler_set((uint32_t)((configCPU_CLOCK_HZ / 1000000) - 1));
	bcm_arm_timer_free_running_counter_set((uint32_t)(1000000 / configTICK_RATE_HZ) - 1);
	bcm_arm_timer_start_running();

	bcm_basic_irq_register(BCM_BASIC_INTERRUPT_ARM_TIMER, (BCM_BASIC_INTERRUPT_HANDLER) FreeRTOS_Tick_Handler, NULL);

	bcm_basic_irq_enable(BCM_BASIC_INTERRUPT_ARM_TIMER);

	portENABLE_INTERRUPTS();

}

void vClearTickInterrupt( void )
{
	/*When reading this register is return 0x544D5241 which is the ASCII reversed value for "ARMT" */ 
	/*The timer IRQ clear register is write only, When writting this register ther interrupt-pennding bit is cleared */
	bcm_arm_timer_clear_irq();
	__asm volatile( "DSB SY" );
	__asm volatile( "ISB SY" );

}

void vApplicationIRQHandler( uint32_t ulICCIAR )
{
	bcm_irq_Handler();
}



