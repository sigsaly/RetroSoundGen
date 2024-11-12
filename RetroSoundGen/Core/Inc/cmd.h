/*
 * cmd.h
 *
 *  Created on: Apr 11, 2024
 *      Author: sela
 */

#ifndef INC_CMD_H_
#define INC_CMD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CMDBUFSIZE  128
#define MAXCMDLEN   128
#define MAXPARAMLEN 64


typedef struct command
{
    char *cmd ;              /* command string (+1 for NUL terminator) */
    int (*cmdfunc)() ;       /* pointer to command function */
}CMD_TBL;

extern const CMD_TBL cmd_tbl[];

char* parse_int(const char *str, int *val, int base);
char *get_token(char *line, char *cmd);
void cmd_loop(void);
int show_help(char *c_ln);
int cmd_exit(char *c_ln);

#ifdef __cplusplus
}
#endif

#endif /* INC_CMD_H_ */
