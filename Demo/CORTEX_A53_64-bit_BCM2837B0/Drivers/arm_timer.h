#ifndef ARM_TIMER_H_
#define ARM_TIMER_H_

void bcm_arm_timer_init (void);
void bcm_arm_timer_pre_scaler_set(uint32_t)
;
void bcm_arm_timer_free_running_counter_set(uint32_t);
void bcm_arm_timer_start_running();
void bcm_arm_timer_clear_irq (void);
void bcm_arm_timer_check_raw_irq(void);
void bcm_arm_timer_check_masked_irq(void);
#endif /* ARM_TIMER_H_ */


