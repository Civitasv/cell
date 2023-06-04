#include "cell/cell.h"
#include "cell/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define CELL_READ_BUFSIZE 1024

char *cell_builtin_cmd[] = { "cd", "help", "exit" };

int (*cell_builtin_func[])(char **) = { &cell_cd, &cell_help, &cell_exit };

static char *cell_read_line()
{
	int buf_size = CELL_READ_BUFSIZE;
	int pos = 0;
	char *buffer = malloc(sizeof(char) * buf_size);
	int c;

	if (!buffer) {
		goto error_malloc;
	}

	while (1) {
		c = getchar();

		if (c == EOF || c == '\n') {
			buffer[pos] = '\0';
			return buffer;
		} else {
			buffer[pos] = c;
		}

		pos++;

		if (pos >= buf_size) {
			buf_size += CELL_READ_BUFSIZE;
			buffer = realloc(buffer, sizeof(char) * buf_size);

			if (!buffer) {
				goto error_malloc;
			}
		}
	}

error_malloc:
	cell_log_error("cell: allocation error\n");
	exit(EXIT_FAILURE);
}

#define CELL_TOKEN_BUFSIZE 64
#define CELL_TOKEN_DELIM " \t\r\n\a"
static char **cell_split_line(char *line)
{
	int buf_size = CELL_TOKEN_BUFSIZE, pos = 0;
	char **tokens = malloc(buf_size * sizeof(char *));
	char *token;

	if (!tokens) {
		goto error_malloc;
	}

	token = strtok(line, CELL_TOKEN_DELIM);
	while (token != NULL) {
		tokens[pos++] = token;

		if (pos >= buf_size) {
			buf_size += CELL_TOKEN_BUFSIZE;
			tokens = realloc(tokens, buf_size * sizeof(char *));

			if (!tokens) {
				goto error_malloc;
			}
		}

		token = strtok(NULL, CELL_TOKEN_DELIM);
	}

	tokens[pos] = NULL;

	return tokens;

error_malloc:
	cell_log_error("cell: allocation error\n");
	exit(EXIT_FAILURE);
}

static int cell_launch(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork();

	if (pid == 0) { // child
		if (execvp(args[0], args) == -1) {
			perror("cell");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) { // error forking
		perror("cell");
	} else { // parent
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
			if (wpid == -1) {
				perror("cell-waitpid");
				exit(EXIT_FAILURE);
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

static int cell_execute(char **args)
{
	int i;
	if (args[0] == NULL) {
		return 1;
	}

	for (i = 0; i < cell_num_builtins(); i++) {
		if (strcmp(args[0], cell_builtin_cmd[i]) == 0) {
			return (*cell_builtin_func[i])(args);
		}
	}

	return cell_launch(args);
}

void cell_loop()
{
	char *line;
	char **args;
	int status;

	do {
		printf("> ");
		line = cell_read_line();
		args = cell_split_line(line);
		status = cell_execute(args);

		free(line);
		free(args);
	} while (status);
}

int cell_num_builtins()
{
	return sizeof(cell_builtin_cmd) / sizeof(char *);
}

int cell_cd(char **args)
{
	if (args[1] == NULL) {
		cell_log_error("cell: expected argument to \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("cell");
		}
	}

	return 1;
}

int cell_help(char **args)
{
	int i;
	printf("Cell\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");

	for (i = 0; i < cell_num_builtins(); i++) {
		printf("  %s\n", cell_builtin_cmd[i]);
	}

	printf("Use the man command for information on other programs.\n");
	return 1;
}

int cell_exit(char **args)
{
	return 0;
}