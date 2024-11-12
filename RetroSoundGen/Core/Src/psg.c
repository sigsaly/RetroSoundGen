/*
 * psg.c
 *
 *  Created on: Jul 17, 2024
 *      Author: sigsaly
 */
#include <stdint.h>
#include <stdbool.h>
#include "main.h"

int dev_id = 0;
int config = 1;
int sustain = 0;

int const midi2tone[2][128] = {
		{15290,14431,13621,12856,12134,11453,10810,10204,9631,9090,
		 8580, 8099, 7644, 7215, 6810, 6428, 6067, 5726,5405,5102,
		 4815, 4545, 4290, 4049, 3822, 3607, 3405, 3214,3033,2863,
		 2702, 2551, 2407, 2272, 2145, 2024, 1911, 1803,1702,1607,
		 1516, 1431, 1351, 1275, 1203, 1136, 1072, 1012, 955, 901,
		  851,  803,  758,  715,  675,  637,  601,  568, 536, 506,
		  477,  450,  425,  401,  379,  357,  337,  318, 300, 284,
		  268,  253,  238,  225,  212,  200,  189,  178, 168, 159,
		  150,  142,  134,  126,  119,  112,  106,  100,  94,  89,
		   84,   79,   75,   71,   67,   63,   59,   56,  53,  50,
		   47,   44,   42,   39,   37,   35,   33,   31,  29,  28,
		   26,   25,   23,   22,   21,   19,   18,   17,  16,  15,
		   14,   14,   13,   12,   11,   11,   10,    9},
		{7645, 7215, 6810, 6428, 6067, 5726, 5405, 5102,4815,4545,
		 4290, 4049, 3822, 3607, 3405, 3214, 3033, 2863,2702,2551,
		 2407, 2272, 2145, 2024, 1911, 1803, 1702, 1607,1516,1431,
		 1351, 1275, 1203, 1136, 1072, 1012,  955,  901, 851, 803,
		  758,  715,  675,  637,  601,  568,  536,  506, 477, 450,
		  425,  401,  379,  357,  337,  318,  300,  284, 268, 253,
		  238,  225,  212,  200,  189,  178,  168,  159, 150, 142,
		  134,  126,  119,  112,  106,  100,   94,   89,  84,  79,
		   75,   71,   67,   63,   59,   56,   53,   50,  47,  44,
		   42,   39,   37,   35,   33,   31,   29,   28,  26,  25,
		   23,   22,   21,   19,   18,   17,   16,   15,  14,  14,
		   13,   12,   11,   11,   10,    9,    9,    8,   8,   7,
		    7,    7,    6,    6,    5,    5,    5,    4}
};


int reset_psg(){
	HAL_GPIO_WritePin(RESETN_GPIO_Port, RESETN_Pin, GPIO_PIN_RESET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(RESETN_GPIO_Port, RESETN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BDIR_1_GPIO_Port, BDIR_1_Pin|BC1_1_Pin, GPIO_PIN_RESET); // to inactive state
}

int latch_address(int8_t id, int8_t address)
{
	uint32_t port, ctrl;
	switch(id)
	{
		case 0:
			port = BDIR_1_GPIO_Port;
			ctrl = BDIR_1_Pin|BC1_1_Pin;
			break;
		case 1:
			port = BDIR_2_GPIO_Port;
			ctrl = BDIR_2_Pin|BC1_2_Pin;
			break;
		case 2:
			port = BDIR_3_GPIO_Port;
			ctrl = BDIR_3_Pin|BC1_3_Pin;
			break;
		case 3:
			port = BDIR_4_GPIO_Port;
			ctrl = BDIR_4_Pin|BC1_4_Pin;
			break;
		case 4:
			port = BDIR_5_GPIO_Port;
			ctrl = BDIR_5_Pin|BC1_5_Pin;
			break;
		case 5:
			port = BDIR_6_GPIO_Port;
			ctrl = BDIR_6_Pin|BC1_6_Pin;
			break;
		case 6:
			port = BDIR_7_GPIO_Port;
			ctrl = BDIR_7_Pin|BC1_7_Pin;
			break;
		case 7:
			port = BDIR_8_GPIO_Port;
			ctrl = BDIR_8_Pin|BC1_8_Pin;
			break;

	}

	DA0_GPIO_Port->ODR = 0;
	HAL_GPIO_WritePin(port, ctrl, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(port, ctrl, GPIO_PIN_SET);
	DA0_GPIO_Port->ODR |= (uint32_t)address;
	HAL_GPIO_WritePin(port, ctrl, GPIO_PIN_RESET);

	return 0;
}

int write_data(int8_t id, int8_t data)
{
	uint32_t port, ctrl;
	DA0_GPIO_Port->ODR = data&0xff;
	switch(id)
	{
		case 0:
			port = BDIR_1_GPIO_Port;
			ctrl = BDIR_1_Pin;
			break;
		case 1:
			port = BDIR_2_GPIO_Port;
			ctrl = BDIR_2_Pin;
			break;
		case 2:
			port = BDIR_3_GPIO_Port;
			ctrl = BDIR_3_Pin;
			break;
		case 3:
			port = BDIR_4_GPIO_Port;
			ctrl = BDIR_4_Pin;
			break;
		case 4:
			port = BDIR_5_GPIO_Port;
			ctrl = BDIR_5_Pin;
			break;
		case 5:
			port = BDIR_6_GPIO_Port;
			ctrl = BDIR_6_Pin;
			break;
		case 6:
			port = BDIR_7_GPIO_Port;
			ctrl = BDIR_7_Pin;
			break;
		case 7:
			port = BDIR_8_GPIO_Port;
			ctrl = BDIR_8_Pin;
			break;
	}
	HAL_GPIO_WritePin(port, ctrl, GPIO_PIN_SET);
	HAL_GPIO_WritePin(port, ctrl, GPIO_PIN_RESET);

	return 0;
}

uint8_t read_data(int8_t id)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	uint32_t data = 0;

	GPIO_InitStruct.Pin = DA0_Pin | DA1_Pin | DA2_Pin | DA3_Pin |
			DA4_Pin | DA5_Pin | DA6_Pin | DA7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DA0_GPIO_Port, &GPIO_InitStruct);
	uint32_t port, ctrl;
	switch(id)
	{
		case 0:
			port = BC1_1_GPIO_Port;
			ctrl = BC1_1_Pin;
			break;
		case 1:
			port = BC1_2_GPIO_Port;
			ctrl = BC1_2_Pin;
			break;
		case 2:
			port = BC1_3_GPIO_Port;
			ctrl = BC1_3_Pin;
			break;
		case 3:
			port = BC1_4_GPIO_Port;
			ctrl = BC1_4_Pin;
			break;
		case 4:
			port = BC1_5_GPIO_Port;
			ctrl = BC1_5_Pin;
			break;
		case 5:
			port = BC1_6_GPIO_Port;
			ctrl = BC1_6_Pin;
			break;
		case 6:
			port = BC1_7_GPIO_Port;
			ctrl = BC1_7_Pin;
			break;
		case 7:
			port = BC1_8_GPIO_Port;
			ctrl = BC1_8_Pin;
			break;
	}
	HAL_GPIO_WritePin(port, ctrl, GPIO_PIN_SET);
	data = DA0_GPIO_Port->IDR;

	GPIO_InitStruct.Pin = DA0_Pin | DA1_Pin | DA2_Pin | DA3_Pin |
			DA4_Pin | DA5_Pin | DA6_Pin | DA7_Pin ;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	HAL_GPIO_Init(DA0_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(port, ctrl, GPIO_PIN_RESET);

	return (uint8_t)data;
}

int write_psg(uint8_t id, uint8_t addr, uint8_t data)
{
	//printf("wr:(%d) %x-%x\n", id, addr, data);
	latch_address(id, addr);
	write_data(id, data);
	return 0;
}

uint8_t read_psg(uint8_t id, uint8_t addr)
{
	latch_address(id, addr);
	return read_data(id);
}


//int enable_port(uint8_t id)
//{
//	write_psg(id, 7, 0xf8);
//}

//int set_vol(uint8_t id, uint8_t port, uint8_t value)
//{
//	uint8_t addr = 8 + port;
//	uint8_t data;
//
//	write_psg(id, addr, value);
//}

int get_note_value(uint8_t id, uint8_t note, uint8_t *coarse, uint8_t *fine)
{
	uint16_t tone_value = midi2tone[id/4][note];
	*coarse = (uint8_t)(tone_value/256);
	*fine =(uint8_t)(tone_value%256);
	return 0;
}


uint8_t modi_vel[] = {0, 4, 5, 7, 8, 9, 9, 10, 11, 12, 12, 13, 13, 14, 14, 15};

int midi_to_tone(uint8_t id, uint8_t port, uint8_t note, uint8_t vel)
{
	uint8_t coarse, fine, addr;

	get_note_value(id, note, &coarse, &fine);
	//printf("note:%d freq: %02x%02x\n", note, coarse, fine);

	addr = port * 2;
	write_psg(id, addr, fine);
	write_psg(id, addr+1, coarse);

	switch(config){
		case 0:
			write_psg(id, port + 8, modi_vel[vel/8]);
			break;
		case 1:
			write_psg(id, port + 8, 0x1f);
			write_psg(id, 13, 0);
			break;
	}
}

#define NUM_PORT 24

struct Port{
	//uint8_t channel;
	uint8_t id;
	uint8_t port;
	bool occupied;
	uint8_t note;
	uint8_t velocity;
} port[NUM_PORT] = {
	{0,0,false,0,0},{1,0,false,0,0},{2,0,false,0,0},{3,0,false,0,0},
	{0,1,false,0,0},{1,1,false,0,0},{2,1,false,0,0},{3,1,false,0,0},
	{0,2,false,0,0},{1,2,false,0,0},{2,2,false,0,0},{3,2,false,0,0},
	{4,0,false,0,0},{5,0,false,0,0},{6,0,false,0,0},{7,0,false,0,0},
	{4,1,false,0,0},{5,1,false,0,0},{6,1,false,0,0},{7,1,false,0,0},
	{4,2,false,0,0},{5,2,false,0,0},{6,2,false,0,0},{7,2,false,0,0},
};

int find_empty_port(uint8_t *id, uint8_t *_port, uint8_t note, uint8_t velocity){
	if(config == 0){
		for(int i = 0; i< NUM_PORT; i++){
			if(!port[i].occupied){
				port[i].occupied = true;
				port[i].note = note;
				port[i].velocity = velocity;
				*_port = port[i].port;
				*id = port[i].id;
				return 0;
			}
		}
	}
	else if(config == 1){
		int i, e;
		if(note >= 64){
			i = 0; e = 12;  // assume right hand
		} else {
			i = 12; e = 24;  // assume left hand
		}
		for(; i < e; i++){
			if(!port[i].occupied){
				port[i].occupied = true;
				port[i].note = note;
				port[i].velocity = velocity;
				*_port = port[i].port;
				*id = port[i].id;
				return 0;
			}
		}
	}
	return -1;
}

int find_assigned_port(uint8_t *id, uint8_t *_port, uint8_t note, uint8_t *velocity ){
	if(config == 0){
		for(int i = 0; i< NUM_PORT; i++){
			if((port[i].occupied) && (port[i].note == note)){
				port[i].occupied = false;
				*_port = port[i].port;
				*id = port[i].id;
				*velocity = port[i].velocity;
				return 0;
			}
		}
	} else if (config == 1){
		int i, e;
		if(note >= 64){
			i = 0; e = 12;  // assume right hand
		} else {
			i = 12; e = 24;  // assume left hand
		}
		for(; i < e; i++){
			if((port[i].occupied)&& (port[i].note == note)){
				port[i].occupied = false;
				*_port = port[i].port;
				*id = port[i].id;
				return 0;
			}
		}
	}
	return -1;
}

void set_midi(uint8_t *data){
	uint8_t msg = data[0] & 0xf0;
	uint8_t channel = data[0] & 0xf;
	uint8_t note = data[1] & 0x7f;
	uint8_t velocity = data[2];
	uint8_t id, port;

	//printf("midi: %02x %02x %02x\n", data[0], data[1], data[2]);
    //printf("msg: %x note: %d vel: %d\n", msg, note, velocity);

	if ((note < MIN_NOTE) || (note > MAX_NOTE))
		return;

	if(channel > 0) return;

	if ((msg == NOTE_ON && velocity >= 0) || msg == NOTE_OFF)
	{

	    if (msg == NOTE_ON && velocity > 0) {
	        if (find_empty_port(&id, &port, note, velocity) >= 0) {
	        	printf("note on id: %d port: %x note: %d vel: %d\n", id, port, note, velocity);
	        	write_psg(id, 0xe, 1); // turn on led
	            midi_to_tone(id, port, note, velocity);
	        }
	    } else {

			if (find_assigned_port(&id, &port, note, &velocity) >= 0) {
				printf("note off id: %d port: %x note: %d vel: %d\n", id, port, note, velocity);
	        	write_psg(id, 0xe, 0); // turn off led
				if(config == 0){
					for (int i = velocity; i > 0; i -= 32) {
						printf("clear i: %d\n", i);
						if (i <= 32) i = 0;
						midi_to_tone(id, port, note, i);
					}
				}else if(config == 1){
					if(sustain < 64){
						//printf("note off, port: %d\n", port + 8);
						write_psg(id, port + 8, 0x4);
						write_psg(id, port + 8, 0x0);
						//write_psg(id, port + 8, 0x0);
					}
				}
	    	}
	    }
	} else if(msg == CONTROL){
		printf("control: %x %x %x\n", msg, note, velocity);
		if(note == SUSTAIN)
			sustain  = velocity;
	}
}

//configuration input on DEV7
uint8_t read_config(){
	return read_psg(7, 0x14);
}

//void init_psg(int config){
void init_psg(){

	//init_port();
	// 24-poly with envelope
	for(int id = 0; id < 8; id++){
		write_psg(id, 7, 0xf8);
		write_psg(id, 8, 0x0);
		write_psg(id, 9, 0x0);
		write_psg(id, 10, 0x0);
		write_psg(id, 11, 0x50);
		write_psg(id, 12, 0x30);
		//write_psg(id, 13, 9);
	}
}


