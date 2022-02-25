/*
 * main.c
 *
 *  Created on: Mar 17 2017
 *      Author: david
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include "dsh.h"
#include "builtins.h"

int main(int argc, char **argv)
{
	int status = 1;
	char cdarg[1];
	cdarg[0] = 'c';
	cdarg[1] = 'd';
	char cmdline[MAXBUF]; // stores user input from commmand line
	dsh_pwd();
	dsh_cd(cdarg);
	dsh_pwd();
	// do {
	// 	printf("dsh> ");
	// 	fgets(cmdline, MAXBUF, stdin);
	// 	if(chkBuiltin(cmdline) != CMD_EXT) {
	// 		if(chkBuiltin(cmdline) == CMD_CD) {
	// 			status = dsh_cd(cmdline);
	// 		}
	// 		else if(chkBuiltin(cmdline) == CMD_EXIT) {
	// 			status = dsh_exit(cmdline);
	// 		}
	// 		else if(chkBuiltin(cmdline) == CMD_PWD) {
	// 			status = dsh_pwd(cmdline);
	// 		}
	// 	}
	// } while(status);

	return 0;
}
