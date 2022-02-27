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
	dsh_loop();
	return 0;
}
