//
// Created by 艾伦 on 2025/11/3.
//

#ifndef HELLO_MY_I2C_H
#define HELLO_MY_I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include <gpio.h>

void my_I2C_start(void );
void my_I2C_end(void );
void my_I2C_sendByte(uint8_t Byte );
uint8_t my_I2C_reciveByte(void );


#endif //HELLO_MY_I2C_H