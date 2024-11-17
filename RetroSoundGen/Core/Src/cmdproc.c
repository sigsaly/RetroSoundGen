/*
 * cmdproc.c
 *
 *  Created on: Apr 11, 2024
 *      Author: sela
 */

#include <stdio.h>
#include <string.h>
#include "cmd.h"
#include "uart.h"

//char cbuf[CMDBUFSIZE];
//char saved_cmd[CMDBUFSIZE];
char cmd_buffer[CMD_BUFFER_SIZE];
char prev_command[CMD_BUFFER_SIZE];
uint8_t cmd_pos = 0;
uint8_t command_mode = 0;

char* parse_int(const char *str, int *val, int base)
{
    if(str[0] == '\0'){
    	return NULL;
    }
    char *endptr;
	*val = (int)strtol(str, &endptr, base);
	return endptr;
}


char *get_token(char *str, char *cmd)
{
	int i;

	// skip blank or else
	while ((*str == ' ') || (*str == '\t'))
	++str;

	for (i = 0; i < MAXCMDLEN; i++)
	{
		// end of line or else
		if ((*str == ' ') || (*str == '\n') || (*str == '\0'))
		break;
		*cmd++ = *str++;
	}
	// add end of string
	*cmd = '\0';

	// return ptr to the next
	return str;
}


int run_cmd(char *cmd_line)
{
	CMD_TBL *p_tbl;
	char cmd[MAXCMDLEN+1];
	char *token, *cmd_ptr;
	char *rem;

	rem = get_token(cmd_line, cmd);

	printf("\n");

	if (!cmd[0]){
		return 0;
	}

	p_tbl = (CMD_TBL *)cmd_tbl;
	cmd_ptr = p_tbl->cmd;
	token = cmd;

	while (p_tbl->cmd)
	{
		if(strcmp(token, cmd_ptr) == 0)
		{
			//strcpy(saved_cmd, cmd_line);
			return ((*p_tbl->cmdfunc)(rem));
		}
		p_tbl++;
		cmd_ptr = p_tbl->cmd;
	}

	if (!p_tbl->cmd)
		printf ("\nInvalid Command! (See Help.)\n");
	return 0;
}

/*
void scan_cmd (char *chr_ptr)
{
	int cnt = 0;
    char ch;

	while(1)
	{
		ch = (char)u_getchar();

        if (ch == '\x1B') {  // ESC
            if (u_getchar() == '[' && u_getchar() == 'A') {  // Up arrow
                strcpy(chr_ptr, saved_cmd);
                printf("\r%s", chr_ptr);
                cnt = strlen(saved_cmd);
            }
            continue;
        }
		if((ch == '\r')||(ch == '\n'))
		{
			if(cnt > 0){
				*chr_ptr = '\0' ;
				strcpy(saved_cmd, chr_ptr);
				break;
			}
		}
		else{
			u_putchar(ch);
			*chr_ptr++ = ch;
			cnt++;
		}
	}
}*/

/*
void cmd_loop(void)
{
	char *cbuf_ptr;
	u_putchar('\n');
	u_putchar('$');
	//__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
	//u_putchar('q');
	while(1) //for (;;)
	{
		cbuf_ptr = cbuf;
		scan_cmd(cbuf_ptr);

		if(run_cmd(cbuf_ptr) < 0){

			__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
			break;
		}
		u_putchar('$');
		//else{
		//	u_putchar('$');
		//}
	}
}*/

int show_help(char *c_ln)
{
	int i=0;

	printf("\n");
	while(cmd_tbl[i].cmd != NULL)
	{
		printf("%s ", cmd_tbl[i].cmd);
		i++;
		if((i%7) == 0)
			printf("\n");
	}
	printf("\n");
	return 0;
}

int cmd_exit(char *c_ln)
{
	printf("Exit command mode\n");
	command_mode = 0;
	return 0;
}

int get_cmd_int(char *c_ln, char *usage)
{
	char index, str_in[MAXCMDLEN];
	int value;

	index = get_token(c_ln, str_in);

	if(str_in[0]==0){
		printf("%s\n", usage);
		return 0;
	}

	if((sscanf(str_in, "%d", &value) != 1)){
		printf("Invalid parameter(s)\n");
		return 0;
	}

	return value;
}
