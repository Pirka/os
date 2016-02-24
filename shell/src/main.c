/*
 * main.c
 *
 *  Created on: Feb 14, 2016
 *      Author: root
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

static char *lsh_read_line(void);
static void lsh_loop(void);
static char **lsh_split_line( char *line );
static int lsh_execute( char **args );

int main( void )
{
	lsh_loop();

	return 0;
}

void lsh_loop(void)
{
  char *line;
  char **args;
  int status = 1;

  while (status)
  {
    printf("$ ");
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  }
}

/*
#define LSH_RL_BUFSIZE 1024
char *lsh_read_line(void)
{
	int bufsize = LSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer)
	{
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		// Read a character
		c = getchar();

		// If we hit EOF, replace it with a null character
		if (c == EOF || c == '\n') {
			buffer[position] = '\0';
			return buffer;
		} else {
			buffer[position] = c;
		}
		position++;

		// If we have exceeded the buffer, reallocate
		if (position >= bufsize) {
			bufsize += LSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer) {
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}*/

char *lsh_read_line( void )
{
	char *line = NULL;
	ssize_t bufsize = 0; // have getline allocate buffer

	(void)getline(&line, &bufsize, stdin);
	return line;
}

/*
int getch( void )
{
	int ch;
	struct termios oldt;
	struct termios newt;

	tcgetattr(STDIN_FILENO, &oldt); // store old settings
	newt = oldt;  // copy old settings to new settings
	newt.c_lflag &= ~(ICANON | ECHO);  // make one change to old settings in new settings
	tcsetattr(STDIN_FILENO, TCSANOW, &newt); // apply the new settings immediately
	ch = getchar(); // standard getchar call
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // reapply the old settings
	return ch;
}*/

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **lsh_split_line( char *line )
{
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "lsh: allocation error\n");
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
	return tokens;
}

int lsh_cd( char **args );
int lsh_help( char **args );
int lsh_exit( char **args );

// List of builtin commands, followed by their corresponding functions
char *builtin_str[] = {
		"cd",
		"help",
		"exit"
};

int (*builtin_func[]) (char **) = {
		&lsh_cd,
		&lsh_help,
		&lsh_exit
};

int lsh_num_builtins()
{
	return sizeof(builtin_str) / sizeof(char*);
}

// builtin function implementations.
int lsh_cd( char **args )
{
	return 1;
}

int lsh_help( char **args )
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

int lsh_exit( char **args )
{
	return 0;
}

int lsh_execute( char **args )
{
	int i;

	if (args[0] == NULL) {
		// an empty command was entered
		return 1;
	}

	for (i=0; i < lsh_num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}

	printf("%s: command not found\n", args[0]);

	return /*lsh_launch(args)*/1;
}

