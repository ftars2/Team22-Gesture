/*
 * debug.h
 *
 *  Created on: Feb 13, 2025
 *      Author: titod
 */

#ifndef DEBUG_H
#define DEBUG_H

#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;  // Declare huart2 globally

void Debug_Init(void);
int __io_putchar(int ch);

#endif /* DEBUG_H */


