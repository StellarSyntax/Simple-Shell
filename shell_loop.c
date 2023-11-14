#include "shell.h"

/**
 * hsh - Main shell loop.
 * @info: The parameter & return info struct.
 * @av: The argument vector from main().
 * Return: 0 on success, 1 on error, or error code.
 */
int hsh(info_t *info, char **av) {
    // Main shell loop
    // ...
}

/**
 * find_builtin - Finds a builtin command.
 * @info: The parameter & return info struct.
 * Return: -1 if builtin not found,
 *         0 if builtin executed successfully,
 *         1 if builtin found but not successful,
 *         -2 if builtin signals exit().
 */
int find_builtin(info_t *info) {
    // Searches for and executes builtin commands
    // ...
}

/**
 * find_cmd - Finds a command in PATH.
 * @info: The parameter & return info struct.
 * Return: void.
 */
void find_cmd(info_t *info) {
    // Finds a command in PATH or executes built-in commands
    // ...
}

/**
 * fork_cmd - Forks an exec thread to run cmd.
 * @info: The parameter & return info struct.
 * Return: void.
 */
void fork_cmd(info_t *info) {
    // Forks a child process to execute a command
    // ...
}
