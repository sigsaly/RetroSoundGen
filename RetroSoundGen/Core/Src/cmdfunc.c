/*
 * cmd_def.c
 *
 *  Created on: Apr 11, 2024
 *      Author: sigsaly
 */
#include "main.h"
#include "cmd.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "markovchain.h"

int cmd_test1();
int cmd_test2();
int cmd_test3();

int cmd_systick();
int cmd_regs();
int cmd_id(char *);
int cmd_write_psg(char *);
int cmd_read_psg(char *);
int cmd_read_all();

int cmd_noise();
int cmd_triad();
int cmd_conf();

int cmd_kick();
int cmd_snare();
int cmd_chihat();
int cmd_ohihat();
int cmd_htom();
int cmd_ltom();
int cmd_crash();
int cmd_ride();

int cmd_mc();

extern int write_psg(uint8_t id, uint8_t addr, uint8_t data);
extern uint8_t read_psg(uint8_t id, uint8_t addr);
extern int write_midi(uint8_t port, uint8_t midi);
extern int enable_aout(uint8_t id, uint8_t port, bool tone, bool en);
extern int set_volume(uint8_t id, uint8_t port, bool envelope_mode, uint8_t value);
extern int get_note_value(uint8_t id, uint8_t note, uint8_t *coarse, uint8_t *fine);
extern int mc_test();

const CMD_TBL cmd_tbl[] =
{
	{"help", show_help},
	{"exit", cmd_exit},
	{"test1", cmd_test1},
	{"test2", cmd_test2},
	{"test3", cmd_test3},

	{"tick", cmd_systick},
	{"regs", cmd_regs},
	{"id", cmd_id},
	{"wr", cmd_write_psg},
	{"rd", cmd_read_psg},
	{"rall", cmd_read_all},
	{"conf", cmd_conf},
	{"noise", cmd_noise},
	{"tri", cmd_triad},


	{"kick", cmd_kick},
	{"snare", cmd_snare},
	{"chh", cmd_chihat},
	{"ohh", cmd_ohihat},
	{"htom", cmd_htom},
	{"ltom", cmd_ltom},
	{"crash", cmd_crash},
	{"ride", cmd_ride},

	{"mc", cmd_mc},
	{0, 0}
};

extern void t_printf(const char *format, ...);
extern void gpio_out(uint8_t data);
//extern void delay_ns(uint32_t ns);
extern int dev_id;
extern int config;

char toLowerCase(char ch) {
    if (ch >= 'A' && ch <= 'Z') {
        ch += 32;
    }
    return ch;
}

int cmd_test1(void){
	write_psg(0, 0, 0xe0);
	write_psg(0, 1, 0x08);
	write_psg(0, 7, 0xf8);
	write_psg(0, 8, 0x1f);
	write_psg(0, 9, 0x0);
	write_psg(0, 10, 0x0);
	write_psg(0, 11, 0x50);
	write_psg(0, 12, 0x20);
	write_psg(0, 13, 9);
}

int cmd_test2(void){
	write_psg(0, 8, 0x1f);
}

int cmd_test3(void){
	write_psg(0, 13, 9);
}

int cmd_systick(){
	static uint32_t saved;
	uint32_t cur_tick = HAL_GetTick();
	printf("tick: %ld, dif: %ld\n", cur_tick, cur_tick - saved);
	saved = cur_tick;
	return 0;
}


int cmd_regs(void){
	printf("R0: Channel A Tone Fine  : D7-D0\n");
	printf("R1: Channel A Tone Coarse: D3-D0\n");
	printf("R2: Channel B Tone Fine  : D7-D0\n");
	printf("R3: Channel B Tone Coarse: D3-D0\n");
	printf("R4: Channel C Tone Fine  : D7-D0\n");
	printf("R5: Channel C Tone Coarse: D3-D0\n");
	printf("R6: Noise Period:          D4-D0\n");
	printf("R7: nEnabled Noise C,B,A : D5,D4,D3\n");
	printf("  : nEnabled Tone  C,B,A : D2,D1,D0\n");
	printf("R8: Channel A Amplitude : D4,D3-D0\n");
	printf("R9: Channel B Amplitude : D4,D3-D0\n");
	printf("Ra: Channel C Amplitude : D4,D3-D0\n");
	printf("Rb: Env Period Fine     : D7-D0\n");
	printf("Rc: Env Period Coarse   : D7-D0\n");
	printf("Rd: Cont,Att,Alt,Hold   : D3,D2,D1,D0\n");
	printf("Re: IOA Data Store      : D7-D0\n");
	printf("Rf: IOB Data Store      : D7-D0\n");
}

int cmd_id(char *c_ln)
{
	if(*c_ln == '\0')
	{
		printf("\ncurrent id: %d\n", dev_id);
		return 0;
	}
	int id = 0;

	c_ln = parse_int(c_ln, &id, 10);
	printf("id: %d\n", id);
	dev_id = id;
	return 0;
}

int cmd_write_psg(char *c_ln)
{
	if(*c_ln == '\0')
	{
		printf("wr addr(hex) data(hex)\n");
		return 0;
	}
	int addr = 0, data = 0;

	c_ln = parse_int(c_ln, &addr, 16);
	if(*c_ln != '\0')
	{
		parse_int(c_ln, &data, 16);
	}

	printf("\nid: %d addr: %d, data: 0x%x\n", dev_id, addr, data);
	write_psg(dev_id, addr, data);
	return 0;
}


int cmd_read_psg(char *c_ln)
{
	int addr = 0;
	if(*c_ln == '\0')
	{
		printf("rd addr(hex)\n");
		return 0;
	}

	parse_int(c_ln, &addr, 16);
	printf("\nid: %d, addr:%x read:%x\n", dev_id, addr, read_psg(dev_id, addr));
	return 0;
}



int cmd_read_all(char *c_ln){
	int id = 0;
	int dat;
	if(*c_ln != '\0')
		parse_int(c_ln, &id, 10);

	for(int i = 0; i < 16; i++){
		dat = read_psg(id, i);
		printf("r%x: %x\n", i, dat);
	}
}



int cmd_conf(char *c_ln){

	int dat = 0;
	if(*c_ln == '\0')
	{
		printf("conf num\n");
		return 0;
	}
	parse_int(c_ln, &dat, 16);
	config = dat;
}


int cmd_noise()
{
	write_psg(dev_id,7,0xf7);
	write_psg(dev_id,8,0xf);
	for(int n = 0; n < 0x1f; n++){
		printf("value:%d, noise freq:%d\n",n, FCLK/16/n);
		HAL_Delay(1000);
		write_psg(dev_id, 6, n);
		write_psg(dev_id,8,0xf);
	}
	write_psg(dev_id,8,0);
}


int cmd_triad()
{
	uint8_t coarse, fine, addr;
	uint8_t tri[] = {60, 65, 55, 60};

	write_psg(dev_id, 7, 0xf8);
	write_psg(dev_id, 8, 0xf);
	write_psg(dev_id, 9, 0xf);
	write_psg(dev_id, 10, 0xf);
	for(int n = 0; n < 4; n++){
		get_note_value(dev_id, tri[n], &coarse, &fine);
		printf("midi num:%d, reg: %x%02x\n", tri[n], coarse, fine);
		write_psg(dev_id, 0, fine);
		write_psg(dev_id, 1, coarse);
		write_psg(dev_id, 8, 0xf);

		//HAL_Delay(1000);
		get_note_value(dev_id, tri[n]+4, &coarse, &fine);
		write_psg(dev_id, 2, fine);
		write_psg(dev_id, 3, coarse);
		write_psg(dev_id, 9, 0xf);

		//HAL_Delay(1000);
		get_note_value(dev_id, tri[n]+7, &coarse, &fine);
		write_psg(dev_id, 4, fine);
		write_psg(dev_id, 5, coarse);
		write_psg(dev_id, 10, 0xf);
		printf("midi num: %d,%d,%d\n", tri[n], tri[n]+4, tri[n]+7);
		HAL_Delay(1000);
	}

	write_psg(dev_id, 8, 0);
	write_psg(dev_id, 9, 0);
	write_psg(dev_id, 10, 0);
}


int cmd_kick(){
	write_psg(dev_id, 0, 0x20);
	write_psg(dev_id, 1, 0xf);
	write_psg(dev_id, 2, 0x50);
	write_psg(dev_id, 3, 0xe);
	write_psg(dev_id, 4, 0xf0);
	write_psg(dev_id, 5, 0xf);
	write_psg(dev_id, 6, 0x1);
	write_psg(dev_id, 7, 0xf8);
	write_psg(dev_id, 8, 0x1f);
	write_psg(dev_id, 9, 0x1f);
	write_psg(dev_id, 10, 0x1f);
	write_psg(dev_id, 11, 0x80);
	write_psg(dev_id, 12, 0x0);
	write_psg(dev_id, 13, 0);
}

int cmd_snare(){
	write_psg(dev_id, 0, 0xa0);
	write_psg(dev_id, 1, 0x7);
	write_psg(dev_id, 2, 0x50);
	write_psg(dev_id, 3, 0x7);
	write_psg(dev_id, 4, 0x80);
	write_psg(dev_id, 5, 0x7);
	write_psg(dev_id, 6, 0x1);
	write_psg(dev_id, 7, 0xc0);
	write_psg(dev_id, 8, 0x1f);
	write_psg(dev_id, 9, 0x1f);
	write_psg(dev_id, 10, 0x1f);
	write_psg(dev_id, 11, 0x50);
	write_psg(dev_id, 12, 0x5);
	write_psg(dev_id, 13, 0);
}

int cmd_chihat(){
	write_psg(dev_id, 6, 0x1);
	write_psg(dev_id, 7, 0xc7);
	write_psg(dev_id, 8, 0x1f);
	write_psg(dev_id, 9, 0x1f);
	write_psg(dev_id, 10, 0x1f);
	write_psg(dev_id, 11, 0x50);
	write_psg(dev_id, 12, 0x5);
	write_psg(dev_id, 13, 0);
}

int cmd_ohihat(){
	write_psg(dev_id, 6, 0x1);
	write_psg(dev_id, 7, 0xc7);
	write_psg(dev_id, 8, 0x1f);
	write_psg(dev_id, 9, 0x1f);
	write_psg(dev_id, 10, 0x1f);
	write_psg(dev_id, 11, 0x50);
	write_psg(dev_id, 12, 0x5);
	write_psg(dev_id, 13, 0);
}

int cmd_htom(){
	write_psg(dev_id, 0, 0x50);
	write_psg(dev_id, 1, 0x10);
	write_psg(dev_id, 2, 0x50);
	write_psg(dev_id, 3, 0x10);
	write_psg(dev_id, 4, 0x50);
	write_psg(dev_id, 5, 0x10);
	write_psg(dev_id, 6, 0x1);
	write_psg(dev_id, 7, 0xc8);
	write_psg(dev_id, 8, 0x1f);
	write_psg(dev_id, 9, 0x1f);
	write_psg(dev_id, 10, 0x1f);
	write_psg(dev_id, 11, 0x50);
	write_psg(dev_id, 12, 0x5);
	write_psg(dev_id, 13, 0);
}

int cmd_ltom(){
	write_psg(dev_id, 0, 0x50);
	write_psg(dev_id, 1, 0x20);
	write_psg(dev_id, 2, 0x50);
	write_psg(dev_id, 3, 0x20);
	write_psg(dev_id, 4, 0x50);
	write_psg(dev_id, 5, 0x20);
	write_psg(dev_id, 6, 0x1);
	write_psg(dev_id, 7, 0xc8);
	write_psg(dev_id, 8, 0x1f);
	write_psg(dev_id, 9, 0x1f);
	write_psg(dev_id, 10, 0x1f);
	write_psg(dev_id, 11, 0x50);
	write_psg(dev_id, 12, 0x5);
	write_psg(dev_id, 13, 0);
}

int cmd_crash(){
	write_psg(dev_id, 6, 0x1);
	write_psg(dev_id, 7, 0xc7);
	write_psg(dev_id, 8, 0x1f);
	write_psg(dev_id, 9, 0x1f);
	write_psg(dev_id, 10, 0x1f);
	write_psg(dev_id, 11, 0x50);
	write_psg(dev_id, 12, 0x5);
	write_psg(dev_id, 13, 0);
}

int cmd_ride(){
	write_psg(dev_id, 6, 0x10);
	write_psg(dev_id, 7, 0xc7);
	write_psg(dev_id, 8, 0x1f);
	write_psg(dev_id, 9, 0x1f);
	write_psg(dev_id, 10, 0x1f);
	write_psg(dev_id, 11, 0x50);
	write_psg(dev_id, 12, 0x5);
	write_psg(dev_id, 13, 0);
}

int cmd_mc(){

}

