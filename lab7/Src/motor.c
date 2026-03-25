
#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdio.h>
#include <stdlib.h>
#include "stm32f0xx.h"

/* -------------------------------------------------------------------------------------------------------------
 *  Global Variable and Type Declarations
 *  -------------------------------------------------------------------------------------------------------------
 */
extern volatile int16_t error_integral;    // Integrated error signal
extern volatile uint8_t duty_cycle;    // Output PWM duty cycle
extern volatile int16_t target_rpm;    // Desired speed target
extern volatile int16_t motor_speed;   // Measured motor speed
extern volatile int8_t adc_value;      // ADC measured motor current
extern volatile int16_t error;         // Speed error signal
extern volatile uint8_t Kp;            // Proportional gain
extern volatile uint8_t Ki;            // Integral gain

static uint8_t buf0[1024];
static uint8_t buf1[1024];
static uint8_t buf2[1024];

void log_init(void) {
    SEGGER_RTT_ConfigUpBuffer(0, "", buf0, 1024, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    SEGGER_RTT_ConfigUpBuffer(1, "", buf1, 1024, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
    SEGGER_RTT_ConfigUpBuffer(2, "", buf2, 1024, SEGGER_RTT_MODE_NO_BLOCK_SKIP);
}

void log_data(void) {
    // Begin critical section
    __disable_irq();
    uint32_t duty_cycle_copy = duty_cycle;
    int32_t target_rpm_copy = target_rpm;
    int32_t motor_speed_copy = motor_speed;
    // End critical section
    __enable_irq();

    union byte_split data;
    data.uword = duty_cycle_copy;
    SEGGER_RTT_Write (0, &data.bytes, 4);
    data.word = target_rpm_copy;
    SEGGER_RTT_Write (1, &data.bytes, 4);
    data.word = motor_speed_copy;
    SEGGER_RTT_Write (2, &data.bytes, 4);
}

/* -------------------------------------------------------------------------------------------------------------
 *  Motor Control and Initialization Functions
 * -------------------------------------------------------------------------------------------------------------
 */

// Sets up the entire motor drive system
void motor_init(void)
{
}

// Set the duty cycle of the PWM, accepts (0-100)
void pwm_setDutyCycle(uint8_t duty);

// PI control code is called within a timer interrupt
void PI_update(void);


/* -------------------------------------------------------------------------------------------------------------
 *  Internal-Use Initialization Functions
 * -------------------------------------------------------------------------------------------------------------
 */

// Sets up the PWM and direction signals to drive the H-Bridge
void pwm_init(void);

// Sets up encoder interface to read motor speed
void encoder_init(void);

// Sets up ADC to measure motor current
void ADC_init(void);

#endif /* MOTOR_H_ */