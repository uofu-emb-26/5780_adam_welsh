#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>


typedef struct {


} My_GPIO_TypeDef;

void My_HAL_GPIO_Init(GPIO_TypeDef  *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    uint32_t position = 0x00u;
    uint32_t iocurrent;
    uint32_t temp;

    while (((GPIO_Init->Pin) >> position) != 0x00u)
    {

        iocurrent = (GPIO_Init->Pin) & (1uL << position);

        if (iocurrent != 0x00u)
        {
            /*--------------------- GPIO Mode Configuration ------------------------*/
            /* In case of Output or Alternate function mode selection */
            if(((GPIO_Init->Mode & GPIO_MODE) == MODE_OUTPUT) ||
                ((GPIO_Init->Mode & GPIO_MODE) == MODE_AF))
            {
               
                /* Configure the IO Speed */
                temp = GPIOx->OSPEEDR;
                temp &= ~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2u));
                temp |= (GPIO_Init->Speed << (position * 2u));
                GPIOx->OSPEEDR = temp;

                /* Configure the IO Output Type */
                temp = GPIOx->OTYPER;
                temp &= ~(GPIO_OTYPER_OT_0 << position) ;
                temp |= (((GPIO_Init->Mode & OUTPUT_TYPE) >> OUTPUT_TYPE_Pos) << position);
                GPIOx->OTYPER = temp;
            }
            
            if((GPIO_Init->Mode & GPIO_MODE) != MODE_ANALOG)
            {
                

                /* Activate the Pull-up or Pull down resistor for the current IO */
                temp = GPIOx->PUPDR;
                temp &= ~(GPIO_PUPDR_PUPDR0 << (position * 2u));
                temp |= ((GPIO_Init->Pull) << (position * 2u));
                GPIOx->PUPDR = temp;
            }

            /* In case of Alternate function mode selection */
            if((GPIO_Init->Mode & GPIO_MODE) == MODE_AF)
            {
                

                /* Configure Alternate function mapped with the current IO */
                temp = GPIOx->AFR[position >> 3u];
                temp &= ~(0xFu << ((position & 0x07u) * 4u));
                temp |= ((GPIO_Init->Alternate) << ((position & 0x07u) * 4u));
                GPIOx->AFR[position >> 3u] = temp;
            }

            /* Configure IO Direction mode (Input, Output, Alternate or Analog) */
            temp = GPIOx->MODER;
            temp &= ~(GPIO_MODER_MODER0 << (position * 2u));
            temp |= ((GPIO_Init->Mode & GPIO_MODE) << (position * 2u));
            GPIOx->MODER = temp;
        }
    position++;
  }

}


/*
void My_HAL_GPIO_DeInit(GPIO_TypeDef  *GPIOx, uint32_t GPIO_Pin)
{
}
*/


GPIO_PinState My_HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    uint16_t output;
    output = GPIOx->IDR;
    output &= GPIO_Pin;
    return (output == 0) ?  GPIO_PIN_RESET : GPIO_PIN_SET;
}



void My_HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
    if (PinState == GPIO_PIN_SET)
    {
        GPIOx->ODR |= GPIO_Pin;
    }
    else{
        GPIOx->ODR &= ~GPIO_Pin;
    }
    //GPIOx->ODR &= ~(1<<GPIO_Pin);
    //GPIOx->ODR |= (PinState << GPIO_Pin);
}



void My_HAL_GPIO_TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->ODR ^= GPIO_Pin;
}

