#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio.h"
#include "stdint.h"

void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();

  tim2_clk_en();

  // TIM_Base_InitTypeDef timBaseString = {.Prescaler = 0x1F3F,
  //                                   .CounterMode = TIM_COUNTERMODE_UP,
  //                                   .Period = 0x00FA};

  // TIM_HandleTypeDef timString = {.Instance = TIM2,
  //                               .Init = timBaseString};
                            
  //TIM3->CR1 |= (1<<0);

  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_InitTypeDef initRegLedString = {GPIO_PIN_8 | GPIO_PIN_9,
                              GPIO_MODE_OUTPUT_PP,
                             GPIO_SPEED_FREQ_LOW,
                               GPIO_NOPULL};

  My_HAL_GPIO_Init(GPIOC, &initRegLedString);

  


  TIM2->PSC = 0x1F3F;
  TIM2->ARR = 0x00FA;
  TIM2->DIER |= (1<<0);
  TIM2->CR1 |= (1<<0);


  NVIC_EnableIRQ(TIM2_IRQn);
  
  My_HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);

  tim3_pwm_setup(0x0014);
  alt_pin_setup();

  TIM3->CR1 |= (1<<0);


  while (1)
  {
    HAL_Delay(500);
    tim3_pwm_setup(0x0010);
    HAL_Delay(500);
    tim3_pwm_setup(0x0008);
    HAL_Delay(500);
    tim3_pwm_setup(0x0001);
    HAL_Delay(500);
    tim3_pwm_setup(0x0014);
    HAL_Delay(500);
    tim3_pwm_setup(0x0032);

  }
  return -1;
}


void TIM2_IRQHandler(void)
{
  My_HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_8 | GPIO_PIN_9);
  TIM2->SR &= ~(1<<0);
}


void tim2_clk_en(void)
{
  RCC->APB1ENR |= (1<< 0);
}

void tim3_pwm_setup(uint32_t duty_cycle)
{
  RCC->APB1ENR |= (1<< 1);
  TIM3->PSC = 0x0063;
  TIM3->ARR = 0x0064;
  TIM3->CCMR1 &= ~((1 << 9) | (1<<8));
  TIM3->CCMR1 &= ~((1<<0) | (1<<1));
  TIM3->CCMR1 &= ~(1<<12);
  TIM3->CCMR1 |= ((1<<14) | (1<<13));
  TIM3->CCMR1 |= ((1<<4) | (1<<5) | (1<<6));
  TIM3->CCMR1 |= ((1<<3) | (1<<11));
  TIM3->CCER |= (1<<0) | (1<<4);
  TIM3->CCR1 = duty_cycle;
  TIM3->CCR2 = duty_cycle;
}

void alt_pin_setup(void){
  GPIOC->MODER &= ~((1<<14) | (1<<12));
  GPIOC->MODER |= (1<<15) | (1<<13);
  

  GPIOC->AFR[0] &= ~((1<<31) | (1<<30) | (1<<29) | (1<<28) | (1<<27) | (1<<26) | (1<<25)| (1<<24));
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
