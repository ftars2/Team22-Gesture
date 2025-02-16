/*
 * debug.c
 *
 *  Created on: Feb 13, 2025
 *      Author: titod
 */

#include "debug.h"
#include "usart.h"  // Include USART header where huart2 is initialized

// Redirect printf() to UART2
int __io_putchar(int ch) {
    HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

void Debug_Init(void) {
    // Ensure USART2 is initialized before using printf()
    MX_USART2_UART_Init();
}


