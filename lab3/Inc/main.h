#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


void Error_Handler(void);
void tim2_clk_en(void);
void alt_pin_setup(void);
void tim3_pwm_setup(uint32_t duty_cycle);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
