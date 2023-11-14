#include "shell.h"

/**
 * retrieveEnvironmentStrings - returns a string array copy of our environment
 * @info: Structure containing potential arguments. Maintains
 *        a constant function prototype.
 * Return: Always 0
 */
char **retrieveEnvironmentStrings(info_t *info)
{
	if (!info->environ || info->env_changed)
	{
		info->environ = convertListToStrings(info->env);
		info->env_changed = 0;
	}

	return (info->environ);
}

/**
 * removeEnvironmentVariable - Remove an environment variable
 * @info: Structure containing potential arguments. Maintains
 *        a constant function prototype.
 * @variable: the string representing the environment variable property
 * Return: 1 on deletion, 0 otherwise
 */
int removeEnvironmentVariable(info_t *info, char *variable)
{
	list_t *node = info->env;
	size_t index = 0;
	char *position;

	if (!node || !variable)
		return (0);

	while (node)
	{
		position = startsWith(node->str, variable);
		if (position && *position == '=')
		{
			info->env_changed = deleteNodeAtIndex(&(info->env), index);
			index = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		index++;
	}
	return (info->env_changed);
}

/**
 * setEnvironmentVariable - Initialize a new environment variable,
 *                          or modify an existing one
 * @info: Structure containing potential arguments. Maintains
 *        a constant function prototype.
 * @variable: the string representing the environment variable property
 * @value: the string representing the environment variable value
 * Return: Always 0
 */
int setEnvironmentVariable(info_t *info, char *variable, char *value)
{
	char *buffer = NULL;
	list_t *node;
	char *position;

	if (!variable || !value)
		return (0);

	buffer = malloc(getStringLength(variable) + getStringLength(value) + 2);
	if (!buffer)
		return (1);
	copyString(buffer, variable);
	concatenateString(buffer, "=");
	concatenateString(buffer, value);
	node = info->env;
	while (node)
	{
		position = startsWith(node->str, variable);
		if (position && *position == '=')
		{
			free(node->str);
			node->str = buffer;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	addNodeEnd(&(info->env), buffer, 0);
	free(buffer);
	info->env_changed = 1;
	return (0);
}
