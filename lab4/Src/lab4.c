#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio.h"

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

  // usart3 clk enable
  RCC->APB1ENR |= (1<<18);


  // setting up alternate pins
  alt_pin_setup();

  // setting baud rate
  uint32_t sys_clk_freq;
  uint16_t baud_reg_val;

  sys_clk_freq = HAL_RCC_GetHCLKFreq();
  baud_reg_val = sys_clk_freq / 115200;

  USART3->BRR = baud_reg_val;



  // enabling transmit and receive
  USART3->CR1 |= (1<<3) | (1<<2);

  USART3->CR1 |= (1<<0);

  char* string = "abc";

  while (1)
  {
    transmit_str(string);
 
  }
  return -1;
}


void alt_pin_setup(void){
  // setting alternate mode to pins 4 and 5
  GPIOC->MODER &= ~((1<<10) | (1<<8));
  GPIOC->MODER |= (1<<11) | (1<<9);
  // setting af1 into pins 4 and 5
  GPIOC->AFR[0] &= ~((1<<23) | (1<<22) | (1<<21) | (1<<19) | (1<<18) | (1<<17) );
  GPIOC->AFR[0] |= ((1<<20) | (1<<16));
}


void transmit_char(char c){
  while(1){
    if((USART3->ISR &= (1<<7)) == 1){
      break; // wait for transmit request
    }
  }
  USART3->TDR = c;
  return;
}

void transmit_str(char* s){
    while(*s != '\0'){
      transmit_char(*s);
      s++;
    }
    return;
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
