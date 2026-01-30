#include "main.h"
#include "stm32f0xx_hal.h"
#include "assert.h"
#include "hal_gpio.h"

void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init(); // reset peripherals
  SystemClock_Config(); // configure sys clock

  /* example uses HAL library calls to control
  the GPIOC peripheral. You'll be redoing this code
  with hardware register access  */

  HAL_RCC_GPIOC_CLK_Enable();
  //__HAL_RCC_GPIOC_CLK_ENABLE();



  // GPIOC->MODER |= (1 << 18) | (1 << 16);
  // assert(GPIOC->MODER == 0x50000 );

  // GPIOC->OTYPER &= ~((1<<9) | (1<<8));
  // assert(GPIOC->OTYPER == 0x0000);

  // GPIOC->OSPEEDR &= ~((1<<18) | (1<<16));
  // assert((GPIOC->OSPEEDR & 0x00050000) == 0x0);

  // GPIOC->PUPDR &= ~((1<<19) | (1<<18) | (1<<17) | (1<<16));
  // assert((GPIOC->PUPDR & 0x000f0000 )== 0x00000000);



  //  GPIO_InitTypeDef initStr = {GPIO_PIN_8 | GPIO_PIN_9,
  //                             GPIO_MODE_OUTPUT_PP,
  //                             GPIO_SPEED_FREQ_LOW,
  //                             GPIO_NOPULL};
  //HAL_GPIO_Init(GPIOC, &initStr);
  // // HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

  // GPIOC->ODR |= (1<<8);
  // assert((GPIOC->ODR & 0x0100 )== 0x100);

  // GPIO_InitTypeDef my_LED_InitString = { GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_7 | GPIO_PIN_6,
  //                                        GPIO_MODE_OUTPUT_PP, 
  //                                        GPIO_NOPULL, 
  //                                        GPIO_SPEED_FREQ_LOW,
  //                                      };

  GPIO_InitTypeDef GPIO_LED_InitStruct = {GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_7 | GPIO_PIN_6,
                              GPIO_MODE_OUTPUT_PP,
                             GPIO_SPEED_FREQ_LOW,
                               GPIO_NOPULL};
  // GPIO_InitTypeDef GPIO_PC6_InitStruct = {
  //   .Pin = GPIO_PIN_6,
  //   .Mode = GPIO_OUTPUT_PP,
  //   .Pull = GPIO_NOPULL,
  //   .Speed = GPIO_SPEED_FREQ_LOW,
  //   .Alternate = NULL
  // };
  // GPIO_InitTypeDef GPIO_PC7_InitStruct = {
  //   .Pin = GPIO_PIN_7,
  //   .Mode = GPIO_OUTPUT_PP,
  //   .Pull = GPIO_NOPULL,
  //   .Speed = GPIO_SPEED_FREQ_LOW,
  //   .Alternate = NULL
  // };

  My_HAL_GPIO_Init(GPIOC, &GPIO_LED_InitStruct);

  //My_HAL_GPIO_Init(GPIOC, NULL);
  //assert(GPIOC->MODER == 0x50000 );

  //  GPIO_InitTypeDef initStr = {GPIO_PIN_8 | GPIO_PIN_9,
  //                             GPIO_MODE_OUTPUT_PP,
  //                             GPIO_SPEED_FREQ_LOW,
  //                             GPIO_NOPULL};
  // HAL_GPIO_Init(GPIOC, &initStr);



  My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);

  while (1) {
    HAL_Delay(200);
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
    My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
    //GPIOC->ODR ^=(1<<9) | (1<<8);
    //HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
  }
}

void HAL_RCC_GPIOC_CLK_Enable(void){
    RCC->AHBENR |= 1<<19;
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add their own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add their own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
