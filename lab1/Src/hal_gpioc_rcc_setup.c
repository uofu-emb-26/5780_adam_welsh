#include <stdint.h>
#include <stm32f0xx_hal.h>

void HAL_RCC_GPIOC_CLK_Enable(void){
    RCC->AHBENR |= 1<<19;
}
