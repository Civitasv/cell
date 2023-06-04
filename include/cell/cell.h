#pragma once

/**
 * @brief main loop for shell
 */
void cell_loop();

/**
 * @brief cd <dir>
 *
 * @param args directory to change
 * @return int status
 */
int cell_cd(char **args);

/**
 * @brief helpful docs for cell
 *
 * @param args cmd to show
 * @return int status
 */
int cell_help(char **args);

/**
 * @brief exit shell process
 *
 * @param args
 * @return int status
 */
int cell_exit(char **args);

extern char *cell_builtin_cmd[];
extern int (*cell_builtin_func[])(char **);
int cell_num_builtins();