#include "main.h"
#include "stm32f0xx_hal.h"

void SystemClock_Config(void);

void config_start_ADC(void);
void config_DAC(void);

// Sine Wave: 8-bit, 32 samples/cycle
const uint8_t sine_table[32] = {127,151,175,197,216,232,244,251,254,251,244,
232,216,197,175,151,127,102,78,56,37,21,9,2,0,2,9,21,37,56,78,102};



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

  // beginning lab 6

  // initializing led pins to output and starting clock
  RCC->AHBENR |= (1<<19);


  GPIO_InitTypeDef GPIO_LED_InitStruct = {GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_7 | GPIO_PIN_6,
                              GPIO_MODE_OUTPUT_PP,
                             GPIO_SPEED_FREQ_LOW,
                               GPIO_NOPULL};
  HAL_GPIO_Init(GPIOC, &GPIO_LED_InitStruct);

  // initilaizng adc pin to addl fn
  GPIOC->MODER |= 0x3;

  // enabling adc clk
  RCC->APB2ENR |= (1<<9);

  // enabling dac clk
  RCC->APB1ENR |= (1<<29);

  // initializing dac pin to addl fn
  GPIOA->MODER |= (0x3<<8);

  config_DAC(); // PA4 DAC_out1

  // ADC config + start

  //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
  config_start_ADC();

  uint8_t read_val;

  uint8_t thresh1 = 40;
  uint8_t thresh2 = 100;
  uint8_t thresh3 = 190;
  uint8_t thresh4 = 230;

  int i = 0;

  while (1)
  {

    if (i == 32)
      i = 0;

    DAC1->DHR8R1 = sine_table[i];
    i++;
    HAL_Delay(1);
    

    // read_val = ADC1->DR;

    // if(read_val > thresh4){
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    // }
    // else if(read_val > thresh3){
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    // }
    // else if(read_val > thresh2){
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);

    // }
    // else if(read_val > thresh1){
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
    // }
    // else{
    //   HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
    // }

    // HAL_Delay(100);
  }
  return -1;
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

void config_DAC(void){
  // PA4 dac_out1

  // set trigger source for channel/output update
  DAC1->CR |= (0x7<<3);
  DAC1->CR |= (1<<0);

}


void config_start_ADC(void){
  // 8 bit res
  ADC1->CFGR1 &= ~(1<<3);
  ADC1->CFGR1 |= (1<<4);

  // cont conversion mode
  ADC1->CFGR1 |= (1<<13);

  // disabling hardware triggers
  ADC1->CFGR1 &= ~(0x3<<10);

  // selecting channel
  ADC1->CHSELR |= (1<<10);

  // starting calibration
  ADC1->CR |= (1<<31);

  // wait for calibration to complete
  while(ADC1->CR & (1<<31)){}

  // enable adc
  ADC1->CR |= (1<<0);

  // wait for ADC ready - ready when adcrdy is high
  while((ADC1->ISR & (1<<0)) == 0){}

  // start conversion
  ADC1->CR |= (1<<2);
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
