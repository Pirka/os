/*
 * shell.c
 *
 *  Created on: Feb 27, 2016
 *      Author: root
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "shellCommands.h"

static char *readLine( void )
{
	char *line = NULL;
	size_t bufsize = 0; // have getline allocate buffer

	getline(&line, &bufsize, stdin);
	return line;
}


#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
static char **splitLine( char *line, int *numTokens )
{
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	if (!numTokens) {
		fprintf(stderr, "lsh:argument error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	*numTokens = position;
	return tokens;
}

int lsh_num_builtins()
{
	return sizeof(builtin_str) / sizeof(char*);
}

int execute( int argc, char **argv )
{
	int i;

	if (argv[0] == NULL) {
		// an empty command was entered
		return 1;
	}

	for (i=0; i < lsh_num_builtins(); i++) {
		if (strcmp(argv[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(argc, argv);
		}
	}

	printf("%s: command not found\n", argv[0]);

	return /*lsh_launch(argv)*/1;
}

void sh_loop(void)
{
  char *line;
  char **argv;
  int argc;
  int status = 1;

  while (status)
  {
    printf("$ ");
    line = readLine();
    argv = splitLine(line, &argc);
    status = execute(argc, argv);

    free(line);
    free(argv);
  }
}


int lsh_help( int argc, char **argv )
{
	int i;
	printf("Irka Shell\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");

	for (i = 0; i < lsh_num_builtins(); i++) {
		printf("    %s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs\n");
	return 1;
}

int lsh_exit( int argc, char **argv )
{
	return 0;
}
