#include "shell.h"

/**
 * bufferInput - buffers chained commands
 * @info: parameter struct
 * @buffer: address of the buffer
 * @length: address of length variable
 *
 * Return: bytes read
 */
ssize_t bufferInput(info_t *info, char **buffer, size_t *length)
{
	ssize_t bytesRead = 0;
	size_t currentLength = 0;

	if (!*length) /* if nothing left in the buffer, fill it */
	{
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, interruptHandler);

		#if USE_GETLINE
			bytesRead = getline(buffer, &currentLength, stdin);
		#else
			bytesRead = customGetline(info, buffer, &currentLength);
		#endif

		if (bytesRead > 0)
		{
			if ((*buffer)[bytesRead - 1] == '\n')
			{
				(*buffer)[bytesRead - 1] = '\0'; /* remove trailing newline */
				bytesRead--;
			}
			info->linecountFlag = 1;
			removeComments(*buffer);
			buildHistoryList(info, *buffer, info->histcount++);

			/* if (_strchr(*buffer, ';')) is this a command chain? */
			{
				*length = bytesRead;
				info->cmdBuffer = buffer;
			}
		}
	}
	return bytesRead;
}

/**
 * getInput - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t getInput(info_t *info)
{
	static char *buffer; /* the ';' command chain buffer */
	static size_t currentIndex, nextIndex, bufferLength;
	ssize_t bytesRead = 0;
	char **bufferPointer = &(info->arg), *position;

	printCharacter(BUF_FLUSH);
	bytesRead = bufferInput(info, &buffer, &bufferLength);

	if (bytesRead == -1) /* EOF */
		return -1;

	if (bufferLength) /* we have commands left in the chain buffer */
	{
		nextIndex = currentIndex; /* init new iterator to the current buffer position */
		position = buffer + currentIndex; /* get pointer for return */

		checkChain(info, buffer, &nextIndex, currentIndex, bufferLength);

		while (nextIndex < bufferLength) /* iterate to semicolon or end */
		{
			if (isChain(info, buffer, &nextIndex))
				break;
			nextIndex++;
		}

		currentIndex = nextIndex + 1; /* increment past nulled ';'' */

		if (currentIndex >= bufferLength) /* reached the end of the buffer? */
		{
			currentIndex = bufferLength = 0; /* reset position and length */
			info->cmdBufferType = CMD_NORMAL;
		}

		*bufferPointer = position; /* pass back pointer to the current command position */
		return _strlen(position); /* return length of the current command */
	}

	*bufferPointer = buffer; /* else not a chain, pass back buffer from customGetline() */
	return bytesRead; /* return length of buffer from customGetline() */
}

/**
 * readBuffer - reads a buffer
 * @info: parameter struct
 * @buffer: buffer
 * @currentIndex: size
 *
 * Return: bytesRead
 */
ssize_t readBuffer(info_t *info, char *buffer, size_t *currentIndex)
{
	ssize_t bytesRead = 0;

	if (*currentIndex)
		return 0;

	bytesRead = read(info->readfd, buffer, READ_BUF_SIZE);

	if (bytesRead >= 0)
		*currentIndex = bytesRead;

	return bytesRead;
}

/**
 * customGetline - gets the next line of input from STDIN
 * @info: parameter struct
 * @pointer: address of pointer to buffer, preallocated or NULL
 * @length: size of the preallocated ptr buffer if not NULL
 *
 * Return: size
 */
int customGetline(info_t *info, char **pointer, size_t *length)
{
	static char buffer[READ_BUF_SIZE];
	static size_t currentIndex, bufferLength;
	size_t k;
	ssize_t bytesRead = 0, size = 0;
	char *currentPointer = NULL, *newPointer = NULL, *character;

	currentPointer = *pointer;

	if (currentPointer && length)
		size = *length;

	if (currentIndex == bufferLength)
		currentIndex = bufferLength = 0;

	bytesRead = readBuffer(info, buffer, &bufferLength);

	if (bytesRead == -1 || (bytesRead == 0 && bufferLength == 0))
		return -1;

	character = _strchr(buffer + currentIndex, '\n');
	k = character ? 1 + (unsigned int)(character - buffer) : bufferLength;
	newPointer = _realloc(currentPointer, size, size ? size + k : k + 1);

	if (!newPointer) /* MALLOC FAILURE! */
		return (currentPointer ? free(currentPointer), -1 : -1);

	if (size)
		_strncat(newPointer, buffer + currentIndex, k - currentIndex);
	else
		_strncpy(newPointer, buffer + currentIndex, k - currentIndex + 1);

	size += k - currentIndex;
	currentIndex = k;
	currentPointer = newPointer;

	if (length)
		*length = size;

	*pointer = currentPointer;
	return size;
}

/**
 * interruptHandler - blocks ctrl-C
 * @sigNum: the signal number
 *
 * Return: void
 */
void interruptHandler(__attribute__((unused))int sigNum)
{
	_puts("\n");
	_puts("$ ");
	printCharacter(BUF_FLUSH);
}
