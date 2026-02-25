#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio.h"

void SystemClock_Config(void);
void I2C_Init(void);
void alt_pin_setup(void);

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

  // setting up gpiob and c clock
  RCC->AHBENR |= (1<<18) | (1<<19);

  alt_pin_setup();

  // set up clock
  RCC->APB1ENR |= (1<<21);

  I2C_Init();

  //enable i2c peripheral
  I2C2->CR1 |= (1<<0);


  while (1)
  {
 
  }
  return -1;
}

void pin_setup(void){
  GPIO_InitTypeDef pin_string_set = { GPIO_PIN_11 | GPIO_PIN_13, GPIO_MODE_AF_OD};
  My_HAL_GPIO_Init(GPIOB, &pin_string_set);

  // setting pin 11 as af1 and pin 13 as af5
  GPIOB->AFR[1] &= ~((1<<15) | (1<<14) | (1<<13) | (1<<23) | (1<<21));
  GPIOB->AFR[1] |= (1<<12) | (1<<22) | (1<<20);

  //initializing pb14 and pc0
  GPIO_InitTypeDef pc0_init = {GPIO_PIN_0, GPIO_MODE_OUTPUT_PP};
  GPIO_InitTypeDef pb14_init = {GPIO_PIN_14, GPIO_MODE_OUTPUT_PP};
  My_HAL_GPIO_Init(GPIOB, &pb14_init);
  My_HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
  My_HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0, GPIO_PIN_SET);
}

void I2C_Init(void){
  // setting timing reg
  I2C1->TIMINGR |= (0x1<<28);
  I2C1->TIMINGR |= 0x13;
  I2C1->TIMINGR |= (0xf)<<8;
  I2C1->TIMINGR |= (0x2)<<16;
  I2C1->TIMINGR |= (0x4)<<20;
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
