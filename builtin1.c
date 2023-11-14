#include "shell.h"

/**
 * displayHistory - shows the command history with line numbers, starting at 0
 * @info: Structure containing potential arguments. Used to maintain
 *        consistent function prototype.
 * Return: Always 0
 */
int displayHistory(info_t *info)
{
	print_list(info->history);
	return (0);
}

/**
 * removeAlias - removes an alias associated with a string
 * @info: parameter structure
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int removeAlias(info_t *info, char *str)
{
	char *equalSignPos, savedChar;
	int result;

	equalSignPos = _strchr(str, '=');
	if (!equalSignPos)
		return (1);
	savedChar = *equalSignPos;
	*equalSignPos = 0;
	result = delete_node_at_index(&(info->alias),
		get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
	*equalSignPos = savedChar;
	return (result);
}

/**
 * defineAlias - defines an alias to a string
 * @info: parameter structure
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int defineAlias(info_t *info, char *str)
{
	char *equalSignPos;

	equalSignPos = _strchr(str, '=');
	if (!equalSignPos)
		return (1);
	if (!*++equalSignPos)
		return (removeAlias(info, str));

	removeAlias(info, str);
	return (add_node_end(&(info->alias), str, 0) == NULL);
}

/**
 * showAlias - displays an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int showAlias(list_t *node)
{
	char *equalSignPos = NULL, *aliasStr = NULL;

	if (node)
	{
		equalSignPos = _strchr(node->str, '=');
		for (aliasStr = node->str; aliasStr <= equalSignPos; aliasStr++)
			_putchar(*aliasStr);
		_putchar('\'');
		_puts(equalSignPos + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * manageAlias - simulates the alias builtin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          consistent function prototype.
 * Return: Always 0
 */
int manageAlias(info_t *info)
{
	int i = 0;
	char *equalSignPos = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			showAlias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		equalSignPos = _strchr(info->argv[i], '=');
		if (equalSignPos)
			defineAlias(info, info->argv[i]);
		else
			showAlias(node_starts_with(info->alias, info->argv[i], '='));
	}

	return (0);
}
