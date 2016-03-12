/*
 * cmds.h
 *
 *  Created on: Mar 11, 2016
 *      Author: root
 */

#ifndef SHELL_SHELLCOMMANDS_H_
#define SHELL_SHELLCOMMANDS_H_

int lsh_cd( int argc, char **argv );
int lsh_ls( int argc, char **argv );
int lsh_cat( int argc, char **argv );
int lsh_help( int argc, char **argv );
int lsh_exit( int argc, char **argv );


// List of builtin commands, followed by their corresponding functions
char *builtin_str[] =
{
	"cd",
	"ls",
	"cat",
	"help",
	"exit"
};

int (*builtin_func[]) (int, char **) = {
	&lsh_cd,
	&lsh_ls,
	&lsh_cat,
	&lsh_help,
	&lsh_exit
};

#endif /* SHELL_SHELLCOMMANDS_H_ */
