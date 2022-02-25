/*
 * dsh.h
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */

#define MAXBUF 256
#define HISTORY_LEN 100

int dsh_cd(char **args);
int dsh_exit(char **args);
int dsh_pwd();
char *readLine();
