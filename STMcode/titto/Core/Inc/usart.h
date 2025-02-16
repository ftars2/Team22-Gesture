/*
 * usart.h
 *
 *  Created on: Feb 13, 2025
 *      Author: titod
 */

#ifndef USART_H
#define USART_H

#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;  // Ensure huart2 is accessible

void MX_USART2_UART_Init(void);

#endif /* USART_H */

