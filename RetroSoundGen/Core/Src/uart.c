/*
 * uart.c
 *
 *  Created on: Apr 11, 2024
 *      Author: sela
 */
#include "uart.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/unistd.h>


#define U_BUF_SIZE (64)
#define MIDI_BUF_SIZE (64)

uint8_t rx_push_pos = 0;
uint8_t rx_pop_pos = 0;
uint8_t rx_data;
uint8_t rx_buff[U_BUF_SIZE];

uint8_t tx_push_pos = 0;
uint8_t tx_pop_pos = 0;
uint8_t tx_data;
uint8_t tx_buff[U_BUF_SIZE];

uint8_t rmidi_push_pos = 0;
uint8_t rmidi_pop_pos = 0;
uint8_t rmidi_data;
uint8_t rmidi_buff[MIDI_BUF_SIZE];

uint8_t tmidi_push_pos = 0;
uint8_t tmidi_pop_pos = 0;
uint8_t tmidi_data;
uint8_t tmidi_buff[MIDI_BUF_SIZE];

#if 0
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//u_putchar('a');
	if(huart->Instance == USART2){
			rmidi_buff [rmidi_push_pos] = rmidi_data;
			if(rmidi_push_pos >= MIDI_BUF_SIZE - 1)
				rmidi_push_pos = 0;
			else
				rmidi_push_pos++;
			HAL_UART_Receive_IT(&huart2, &rmidi_data, 1);
		}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
}
#endif


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) {
        // 수신된 MIDI 데이터를 수신 버퍼에 저장
        rmidi_buff[rmidi_push_pos] = rmidi_data;

        // 송신 버퍼에도 동일한 데이터를 저장하여 리피트 준비
        tmidi_buff[tmidi_push_pos] = rmidi_data;

        // 수신 위치 포인터 업데이트
        if (rmidi_push_pos >= MIDI_BUF_SIZE - 1)
            rmidi_push_pos = 0;
        else
            rmidi_push_pos++;

        // 송신 위치 포인터 업데이트
        if (tmidi_push_pos >= MIDI_BUF_SIZE - 1)
            tmidi_push_pos = 0;
        else
            tmidi_push_pos++;

        // 다음 수신 대기
        HAL_UART_Receive_IT(&huart2, &rmidi_data, 1);

        // 송신 인터럽트 활성화
        HAL_UART_Transmit_IT(&huart2, &tmidi_buff[tmidi_pop_pos], 1);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) {
        // 다음 송신할 데이터가 있는지 확인
        if (tmidi_pop_pos != tmidi_push_pos) {
            // 다음 데이터 송신
            if (tmidi_pop_pos >= MIDI_BUF_SIZE - 1)
                tmidi_pop_pos = 0;
            else
                tmidi_pop_pos++;

            HAL_UART_Transmit_IT(&huart2, &tmidi_buff[tmidi_pop_pos], 1);
        }
    }
}

void uart_proc(void)
{
	static uint8_t midi_data[3];
	static int midi_index = 0;

	while(rmidi_push_pos != rmidi_pop_pos)
	{
		uint8_t ch = rmidi_buff[rmidi_pop_pos];
		if(rmidi_pop_pos >= MIDI_BUF_SIZE - 1)
			rmidi_pop_pos = 0;
		else
			rmidi_pop_pos++;

		if(ch & 0x80){
			midi_index = 0;
			midi_data[midi_index] = ch;
			midi_index++;
			//u_printf("status: %02x\n", ch);
		}else{
			if(midi_index <= 2){
				midi_data[midi_index] = ch;
				midi_index++;
				if(midi_index == 3){
					set_midi(midi_data);
					midi_index = 0;
				}
			}else{
				printf("Error: midi_index: %d\n", midi_index);
			}
			//printf("data: %02x\n", ch);
		}

	}

	if(tmidi_push_pos != tmidi_pop_pos)
	{
		if(huart2.gState == HAL_UART_STATE_READY)
		{
			uint8_t ch = tmidi_buff[tmidi_pop_pos];
			if(tmidi_pop_pos >= MIDI_BUF_SIZE - 1)
				tmidi_pop_pos = 0;
			else
				tmidi_pop_pos++;
			HAL_UART_Transmit_IT(&huart2, (uint8_t *)&ch, 1);
		}
	}
#if 0
	/* echo received chars */
	while(rx_push_pos != rx_pop_pos)
	{
		uint8_t ch = rx_buff[rx_pop_pos];
		if(rx_pop_pos >= U_BUF_SIZE - 1)
			rx_pop_pos = 0;
		else
			rx_pop_pos++;
		u_putchar(ch);
	}

	if(tx_push_pos != tx_pop_pos)
	{
		if(huart1.gState == HAL_UART_STATE_READY)
		{
			uint8_t ch = tx_buff[tx_pop_pos];
			if(tx_pop_pos >= U_BUF_SIZE - 1)
				tx_pop_pos = 0;
			else
				tx_pop_pos++;
			HAL_UART_Transmit_IT(&huart1, (uint8_t *)&ch, 1);
		}
	}
#endif
}

int u_kbhit()
{
    if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
    {
        return 1;
    }
    return 0;
}

char u_getchar()
{
    uint8_t data = 0;
    //printf("u_get\n");
    HAL_UART_Receive(&huart1, &data, 1, HAL_MAX_DELAY);
    //printf("char\n");
    return data;
}

void u_putchar(char c)
{
	//if(huart1.gState == HAL_UART_STATE_READY)
	//if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE))
	while(!__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE));

	{
		HAL_UART_Transmit(&huart1, (uint8_t*)&c, 1, 0); //0);
	}
}

int _write(int file, char *data, int len)
{
    if ((file != STDOUT_FILENO) && (file != STDERR_FILENO))
    {
        return -1;
    }

    HAL_UART_Transmit(&huart1, (uint8_t*)data, len, HAL_MAX_DELAY);
    return len;
}

void u_printf(const char* format, ...)
{
	va_list arg;
	va_start(arg, format);
	vprintf(format, arg);
	va_end(arg);
}

void t_printf(const char *format, ...)
{
    static uint32_t last_tick = 0;
    char new_format[1024];

    uint32_t cur = HAL_GetTick();
    int dif = cur - last_tick;
    int s = dif/1000;
    int ms = dif - s*1000;
    snprintf(new_format, 1024, "%d.%d:%s",s, ms, format);

    va_list args;
    va_start(args, new_format);
    vprintf(new_format, args);
    va_end(args);
    last_tick = cur;
}

