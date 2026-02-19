#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

void Error_Handler(void);
void alt_pin_setup(void);
void transmit_char(char c);
void transmit_str(char* s);
void USART3_4_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
