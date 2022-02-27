/*
 * dsh.h
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */

#define MAXBUF 256
#define DELIM " \t\r\n\a"
#define HISTORY_LEN 100

int dsh_cd(char **args);
int dsh_exit();
int dsh_pwd(char **args);
char **splitLine(char *line);
void dsh_loop(void);
int dsh_execute(char **args, int conc);
int dsh_launch(char **args);
int dsh_launch_conc(char **args);