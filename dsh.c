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
        if(chdir(args[1]) != 0) {
            perror("dsh");
        }
    }
    return 1;
}

//builtin function to terminate dsh.
int dsh_exit() {
    return 0;
}

//builtin function to print the user's current working directory
int dsh_pwd(char **args) {
    char cwd[256];
    getcwd(cwd, 256);
    printf("%s\n", cwd);
    return 1;
}

/**
 * Splits the user input into tokens for commands that require several arguments
 * @param line the input the user typed into the command line
 * @return a pointer to an array of argument tokens which will then be passed to
 * the execute function
 */
char **splitLine(char *line){
    int position = 0;
    char **tokens = (char**) malloc(64 * sizeof(char*));
    char *token;
    if(!tokens){
        fprintf(stderr, "dsh: allocation error\n");
        exit(1);
    }
    token = strtok(line, DELIM);
    while(token != NULL){
        tokens[position] = (char*) malloc(128 * sizeof(char));
        strcpy(tokens[position], token);
        position++;
        if(position >= 64){
            tokens = realloc(tokens, 128 * sizeof(char*));
            if(!tokens){
                fprintf(stderr, "dsh: allocation error\n");
                exit(1);
            }
        }
        token = strtok(NULL, DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

//Main loop to prompt for user input
void dsh_loop(void) {
    int status;
    int conc; //variable to keep track of whether to run command concurrently or not when passed to execute function
	char cmdline[MAXBUF]; // stores user input from commmand line

	 FILE* motd = fopen(".dsh_motd", "r"); //open the motd file if it exists
        char c;
        if(motd != NULL) {
            while(c != EOF) {
                c = fgetc(motd);
                if(c != EOF) {
                    printf("%c", c);
                }
            }
            printf("\n");
            fclose(motd);
        }
    do {
		printf("dsh> ");
		fgets(cmdline, MAXBUF, stdin);
		cmdline[strcspn(cmdline, "\n")] = 0; //remove trailing '\n' from fgets()
        size_t n = sizeof(cmdline)/sizeof(cmdline[0]);
        if(cmdline[n] == '&') {  //check if user input ends with '&' to determine whether to
            conc = 0;            //run their command concurrently or not
        }
        else {
            conc = 1;
        }
		char **cmdarg = splitLine(cmdline); //split input into tokens
        status = dsh_execute(cmdarg, conc);
	} while(status);
}

/**
 * Accepts parsed user input and executes the given commnds
 * @param args a pointer to the array of command tokens
 * @param conc value to determine whether to launch a program concurrently with dsh or not. will either 
 * be 0 or 1.
 * @return status to keep the main loop going
 */
int dsh_execute(char **args, int conc) {

    if(args[0] == NULL) { //empty command entered, return 1 to keep loop running
        return 1;
    }
    cmd_t cmd = chkBuiltin(args[0]);
    if(cmd != CMD_EXT) {              //check if command corresponds to a builtin function
		if(cmd == CMD_CD) {
			return dsh_cd(args);
		}
		else if(cmd == CMD_EXIT) {
			return dsh_exit(args);
		}
		else if(cmd == CMD_PWD) {
			return dsh_pwd(args);
		}
	}
    if(conc == 0) {                   //command not a builtin, attempt to launch program using given arguments
        return dsh_launch_conc(args); //launch and run concurrently with dsh
    }
    else {
        return dsh_launch(args);  //launch and wait for program to terminate before continuing dsh
    } 
}

//Takes in the parsed arguments and attempts to execute them
//Waits for the child process to finish running before continuing
//execution of dsh.
int dsh_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0) {
        if(execvp(args[0], args) == -1) {
            perror("dsh");
        }
        exit(1);
    }
    else if(pid < 0) {
        perror("dsh");
    }
    else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

//Takes in the parsed arguments and attempts to execute them
//Does not wait for child process to terminate, and runs the given
//command(s) concurrently with dsh.
int dsh_launch_conc(char **args) {
    pid_t pid;

    pid = fork();
    if(pid == 0) {
        if(execvp(args[0], args) == -1) {
            perror("dsh");
        }
        exit(1);
    }
    else if(pid < 0) {
        perror("dsh");
    }
    return 1;
}