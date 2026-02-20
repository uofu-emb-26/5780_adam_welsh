#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio.h"
#include "stdint.h"

void SystemClock_Config(void);


volatile char receivedData;
volatile char data_flag;

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

  __HAL_RCC_GPIOC_CLK_ENABLE();


 // setting up debugging led's
  GPIO_InitTypeDef initString = {GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_7 | GPIO_PIN_6,
                              GPIO_MODE_OUTPUT_PP,
                             GPIO_SPEED_FREQ_LOW,
                               GPIO_NOPULL};

  HAL_GPIO_Init(GPIOC, &initString);


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
  USART3->CR1 |= (1<<5);

  NVIC_EnableIRQ(USART3_4_IRQn);

  NVIC_SetPriority(USART3_4_IRQn, 1);

  while (1)
  {
    // while(!(USART3->ISR &= (1<<5))){
    // }

    if(data_flag == 1){
      switch (receivedData){
      case 'r':
        data_flag = 0;
        transmit_str("What is your cmd?");
        LED_SRT_Hanlder(GPIO_PIN_6);
        break;
      case 'g':
        data_flag = 0;
        LED_SRT_Hanlder(GPIO_PIN_9);
        break;
      case 'b':
        data_flag = 0;
        LED_SRT_Hanlder(GPIO_PIN_7);
        break;
      case 'o':
        data_flag = 0;
        LED_SRT_Hanlder(GPIO_PIN_8);
        break;
      default:
        transmit_str("Error: Input char doesn't match supported cases\r\n");
        break;
      }

      data_flag = 0;
    }

    
    

  }

  return -1;
}


void LED_SRT_Hanlder(uint16_t GPIO_Pin){
  while(data_flag != 1){
  }

  if(receivedData == '0'){
      HAL_GPIO_WritePin(GPIOC, GPIO_Pin, GPIO_PIN_RESET);
      transmit_str("Reset pin\r\n");
      }
    else if(receivedData == '1'){
      HAL_GPIO_WritePin(GPIOC, GPIO_Pin, GPIO_PIN_SET);
      transmit_str("Set pin\r\n");
      }
    else if(receivedData == '2'){
      HAL_GPIO_TogglePin(GPIOC, GPIO_Pin);
      transmit_str("Toggled pin\r\n");
      }
    else
      transmit_str("Error: cmd must give valid number\r\n");
}

void USART3_4_IRQHandler(void){
  receivedData = USART3->RDR;
  data_flag = 1;
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
    if((USART3->ISR &= (1<<7))){
     // My_HAL_
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
