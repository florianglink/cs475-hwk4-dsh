/*
 * dsh.c
 *
 *  Created on: Aug 2, 2013
 *      Author: chiu
 */
#include "dsh.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <errno.h>
#include <err.h>
#include <sys/stat.h>
#include <string.h>
#include "builtins.h"

// TODO: Your function definitions (declarations in dsh.h)

//builtin function to change current working directory.
//If no path is given, change to home directory.
int dsh_cd(char **args) {
    if(args[1] == NULL) {
        chdir(getenv("HOME"));
    }
    else {
        if(chdir(args[1] != 0)) {
            perror("dsh: unable to change directory with given path");
        }
    }
    return 1;
}

//builtin function to terminate dsh.
int dsh_exit(char **args) {
    return 0;
}

//builtin function to print the user's current working directory
int dsh_pwd() {
    char cwd[256];
    getcwd(cwd, 256);
    printf("%s\n", cwd);
    return 1;
}

//reads and returns the user input from the command line
char *readLine() {
    char cmdline[MAXBUF];
    fgets(cmdline, MAXBUF, stdin);
    printf("input received: %s", cmdline);
    return cmdline;
}