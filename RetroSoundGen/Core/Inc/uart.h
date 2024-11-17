/*
 * usrt.h
 *
 *  Created on: Apr 11, 2024
 *      Author: sela
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern uint8_t rx_data;
extern uint8_t rmidi_data;
void uart_proc(void);
void uart_init();
//int u_kbhit();
char u_getchar();
void u_putchar(char c);
//void u_printf(const char* format, ...);
void t_printf(const char *format, ...);
#endif /* INC_UART_H_ */
