/*
 * main.c
 *
 *  Created on: Feb 14, 2016
 *      Author: root
 */


#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

static char *lsh_read_line(void);
static void lsh_loop(void);
static char **lsh_split_line( char *line, int *numTokens );
static int lsh_execute( int argc, char **argv );

int main( void )
{
  FILE *pFile;
  uint8_t data[512];
  int bytesRead = 0;
  int i,j;
  uint16_t address=0;

  pFile = fopen("block_2.bin", "rb");
  if (pFile)
  {
	  bytesRead = fread(data, 1, 512, pFile);

	  for (i=0,address=0; i < 512/16; i++)
	  {
		printf("%04X: ", address);
		address += 16;

	    for (j=0; j < 16; j++)
	    {
		  printf("%02X ", data[(i*16)+j]);
	    }
	    printf("\n");
	  }
  }

  printf("\n");
  fflush(stdout);


	lsh_loop();

	return 0;
}

void lsh_loop(void)
{
  char *line;
  char **argv;
  int argc;
  int status = 1;

  while (status)
  {
    printf("$ ");
    line = lsh_read_line();
    argv = lsh_split_line(line, &argc);
    status = lsh_execute(argc, argv);

    free(line);
    free(argv);
  }
}

char *lsh_read_line( void )
{
	char *line = NULL;
	size_t bufsize = 0; // have getline allocate buffer

	getline(&line, &bufsize, stdin);
	return line;
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **lsh_split_line( char *line, int *numTokens )
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

int lsh_cd( int argc, char **argv );
int lsh_help( int argc, char **argv );
int lsh_exit( int argc, char **argv );

// List of builtin commands, followed by their corresponding functions
char *builtin_str[] = {
		"cd",
		"help",
		"exit"
};

int (*builtin_func[]) (int, char **) = {
		&lsh_cd,
		&lsh_help,
		&lsh_exit
};

int lsh_num_builtins()
{
	return sizeof(builtin_str) / sizeof(char*);
}

// builtin function implementations.
int lsh_cd( int argc, char **argv )
{
	return 1;
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

int lsh_execute( int argc, char **argv )
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

