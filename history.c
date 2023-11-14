#include "shell.h"

/**
 * obtainHistoryFile - obtains the history file path
 * @info: parameter struct
 *
 * Return: allocated string containing the history file path
 */
char *obtainHistoryFile(info_t *info)
{
	char *path, *directory;

	directory = _getenv(info, "HOME=");
	if (!directory)
		return (NULL);

	path = malloc(sizeof(char) * (_strlen(directory) + _strlen(HIST_FILE) + 2));
	if (!path)
		return (NULL);

	path[0] = 0;
	_strcpy(path, directory);
	_strcat(path, "/");
	_strcat(path, HIST_FILE);

	return (path);
}

/**
 * recordHistory - creates or appends to a history file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int recordHistory(info_t *info)
{
	ssize_t fileDescriptor;
	char *filename = obtainHistoryFile(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fileDescriptor = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);

	if (fileDescriptor == -1)
		return (-1);

	for (node = info->history; node; node = node->next)
	{
		_putsToFileDescriptor(node->str, fileDescriptor);
		_putCharToFileDescriptor('\n', fileDescriptor);
	}

	_putCharToFileDescriptor(BUF_FLUSH, fileDescriptor);
	close(fileDescriptor);

	return (1);
}

/**
 * retrieveHistory - reads history from a file
 * @info: the parameter struct
 *
 * Return: history count on success, 0 otherwise
 */
int retrieveHistory(info_t *info)
{
	int i, last = 0, lineCount = 0;
	ssize_t fileDescriptor, readLength, fileSize = 0;
	struct stat statistics;
	char *buffer = NULL, *filename = obtainHistoryFile(info);

	if (!filename)
		return (0);

	fileDescriptor = open(filename, O_RDONLY);
	free(filename);

	if (fileDescriptor == -1)
		return (0);

	if (!fstat(fileDescriptor, &statistics))
		fileSize = statistics.st_size;

	if (fileSize < 2)
		return (0);

	buffer = malloc(sizeof(char) * (fileSize + 1));

	if (!buffer)
		return (0);

	readLength = read(fileDescriptor, buffer, fileSize);
	buffer[fileSize] = 0;

	if (readLength <= 0)
		return (free(buffer), 0);

	close(fileDescriptor);

	for (i = 0; i < fileSize; i++)
		if (buffer[i] == '\n')
		{
			buffer[i] = 0;
			buildHistoryList(info, buffer + last, lineCount++);
			last = i + 1;
		}

	if (last != i)
		buildHistoryList(info, buffer + last, lineCount++);

	free(buffer);
	info->histCount = lineCount;

	while (info->histCount-- >= HIST_MAX)
		deleteNodeAtIndex(&(info->history), 0);

	adjustHistoryNumbers(info);

	return (info->histCount);
}

/**
 * constructHistoryList - adds an entry to the history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buffer: buffer
 * @lineCount: the history line count, histCount
 *
 * Return: Always 0
 */
int constructHistoryList(info_t *info, char *buffer, int lineCount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;

	addNodeEnd(&node, buffer, lineCount);

	if (!info->history)
		info->history = node;

	return (0);
}

/**
 * renumberHistory - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histCount
 */
int renumberHistory(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}

	return (info->histCount = i);
}
