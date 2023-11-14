#include "shell.h"

/**
 * initializeInfo - initializes the info_t struct
 * @info: struct address
 */
void initializeInfo(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * populateInfo - initializes the info_t struct
 * @info: struct address
 * @argumentVector: argument vector
 */
void populateInfo(info_t *info, char **argumentVector)
{
	int index = 0;

	info->fname = argumentVector[0];
	if (info->arg)
	{
		info->argv = splitString(info->arg, " \t");
		if (!info->argv)
		{
			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = duplicateString(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (index = 0; info->argv && info->argv[index]; index++)
			;
		info->argc = index;

		replaceAliases(info);
		replaceVariables(info);
	}
}

/**
 * releaseInfo - frees info_t struct fields
 * @info: struct address
 * @freeAll: true if freeing all fields
 */
void releaseInfo(info_t *info, int freeAll)
{
	freeDynamicMemory(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (freeAll)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			freeList(&(info->env));
		if (info->history)
			freeList(&(info->history));
		if (info->alias)
			freeList(&(info->alias));
		freeDynamicMemory(info->environ);
		info->environ = NULL;
		freeBuffer((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		printCharacter(BUF_FLUSH);
	}
}
