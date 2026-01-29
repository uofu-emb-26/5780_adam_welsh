#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{

    if (GPIOx == GPIOC){
        GPIOx->MODER |= (1 << 18) | (1 << 16) | (1<<14) | (1<<12);

        GPIOx->OTYPER &= ~((1<<9) | (1<<8) | (1<<7) | (1<<6));

        GPIOx->OSPEEDR &= ~((1<<18) | (1<<16) | (1<<14) | (1<<12));

        GPIOx->PUPDR &= ~((1<<19) | (1<<18) | (1<<17) | (1<<16) | (1<<15) | (1<<14) | (1<<13) | (1<<12));
    }
    else if (GPIOx == GPIOA) {
        GPIOx->MODER &= ~((1<<1) | (1<<0));
        GPIOx->OSPEEDR &= ~(1<<0);
        GPIOx->PUPDR &= ~(1<<0);
        GPIOx->PUPDR |= (1<<1);
    }
}


/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/

/*
GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    return -1;
}
*/


void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    GPIOx->ODR |= (1<<GPIO_Pin);

}



void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
}

