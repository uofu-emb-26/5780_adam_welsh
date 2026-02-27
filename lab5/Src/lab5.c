#include "main.h"
#include "stm32f0xx_hal.h"
#include "hal_gpio.h"

#define WRITE 0
#define READ 1

#define RESTART 1
#define NO_RESTART 0


#define THRESHOLD 500

void SystemClock_Config(void);
void I2C_Init(void);
void pin_setup(void);

void init_i2c_trans(char rw, int num_bytes);
int wait_txis();
void wait_tc();
int wait_rxne();
void stop_i2c();


int write_i2c(char* msg, int len, char restart);
char* read_i2c (int num_bytes, char* buffer);
int length (char* bytes);

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

  // setting up leds for debugging:

  GPIO_InitTypeDef GPIO_LED_InitStruct = {GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_7 | GPIO_PIN_6,
                              GPIO_MODE_OUTPUT_PP,
                             GPIO_SPEED_FREQ_LOW,
                               GPIO_NOPULL};
  HAL_GPIO_Init(GPIOC, &GPIO_LED_InitStruct);

  pin_setup();

  // set up clock
  RCC->APB1ENR |= (1<<22);

  I2C_Init();

  //enable i2c peripheral
  I2C2->CR1 |= (1<<0);

  //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);


  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
  HAL_Delay(1000);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_RESET);

  char msg[2] = {0x0f};
  char buffer1[1];
  write_i2c(msg, 1, RESTART);
  char result = *(read_i2c(1, (char *)buffer1)); // should contain 0xd3

  if(result == 0xd3){
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_SET);
    HAL_Delay(1000);
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,GPIO_PIN_RESET);

  }

  // checkoff 2

  uint8_t buffer2[2];

  // set control register
  msg[0] = 0x20; // address
  msg[1] = 0x0b; // info
  write_i2c(msg, 2, NO_RESTART);

  while (1)
  {

  // Read X
  msg[0] = 0xA8;   // auto-increment + X_L
  write_i2c(msg, 1, RESTART);

  read_i2c(2, (char*)buffer2);
  int16_t x_data = (int16_t)((buffer2[1] << 8) | buffer2[0]);

  // Read Y
  msg[0] = 0xAA;
  write_i2c(msg, 1, RESTART);

  read_i2c(2, (char*)buffer2);
  int16_t y_data = (int16_t)((buffer2[1] << 8) | buffer2[0]);

  if(x_data < -THRESHOLD){
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
  }
  else if(x_data > THRESHOLD){
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
  }

  if(y_data < -THRESHOLD){
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
  }
  else if(y_data > THRESHOLD){
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
  }

  HAL_Delay(100);

 
  }
  return -1;
}

void pin_setup(void){
  GPIO_InitTypeDef pin_string_set = { GPIO_PIN_11 | GPIO_PIN_13, GPIO_MODE_AF_OD};
  HAL_GPIO_Init(GPIOB, &pin_string_set);

  // setting pin 11 as af1 and pin 13 as af5
  GPIOB->AFR[1] &= ~((1<<15) | (1<<14) | (1<<13) | (1<<23) | (1<<21));
  GPIOB->AFR[1] |= (1<<12) | (1<<22) | (1<<20);

  //initializing pb14 and pc0
  GPIO_InitTypeDef pc0_init = {GPIO_PIN_0, GPIO_MODE_OUTPUT_PP};
  GPIO_InitTypeDef pb14_init = {GPIO_PIN_14, GPIO_MODE_OUTPUT_PP};
  HAL_GPIO_Init(GPIOB, &pb14_init);
  HAL_GPIO_Init(GPIOC, &pc0_init);

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0, GPIO_PIN_SET);
}

void I2C_Init(void){
  // setting timing reg
  I2C2->TIMINGR |= (0x1<<28);
  I2C2->TIMINGR |= 0x13;
  I2C2->TIMINGR |= (0xf)<<8;
  I2C2->TIMINGR |= (0x2)<<16;
  I2C2->TIMINGR |= (0x4)<<20;
}

/*
  rw (0 - write, 1 - read)
 */
void init_i2c_trans(char rw, int num_bytes){
    // clrearing nbytes and sadd and setting them
  I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
  // 1 byte to send (first part of message), slave address 0x6b
  I2C2->CR2 |= (num_bytes<<16) | (0x69<<1);

  if(rw)
    I2C2->CR2 |= (1<<10); // set bit for read
  else
    I2C2->CR2 &= ~(1<<10); // clear bit for write

  // set start bit
  I2C2->CR2 |= (1<<13);

}

int wait_txis(){
    // block while nackf and txis are zero
  while (( I2C2->ISR & ((1<<4) | (1<<1))) == 0 ){}

  if( I2C2->ISR & (1<<4)){
    return -1;
  }
  return 0;
}


void wait_tc(){
  // wait until TC is set
  while( !(I2C2->ISR & (1<<6))){}
  return;
}



int wait_rxne(){

  while((I2C2->ISR & ((1<<4) | (1<<2))) == 0 ){}

  if( I2C2->ISR & (1<<4)){
    return -1;
  } // continue normally if nackf not set
  return 0;
}


int write_i2c(char *msg, int len, char restart){
 

  init_i2c_trans(WRITE, len);
 

  // send whole message
  for (int i = 0; i < len; i++) {

    if (wait_txis() == -1) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
        return -1;
    }

    I2C2->TXDR = msg[i];
}

  wait_tc();

  if(!restart)
    stop_i2c();

  return 0;
}

void stop_i2c(){
 // send stop condition
  I2C2->CR2 |= (1<<14);
}

char* read_i2c (int num_bytes, char* buffer){

  init_i2c_trans(READ, num_bytes);

  if( wait_rxne() == -1){
    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6,GPIO_PIN_SET);
    return NULL;
  }


  for (int i = 0; i < num_bytes; i++) {
    if (wait_rxne() == -1) {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
        return NULL;
    }

    buffer[i] = I2C2->RXDR;
}

  wait_tc();

  stop_i2c();

  return buffer;
}

int length (char* bytes){
  int count = 0;
  while (bytes[count] != '\0'){
    count++;
  }
  return count;
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
