#include "shell.h"

/**
 * printErrorMessage - prints an input error message
 * @message: the error message to be printed
 *
 * Return: Nothing
 */
void printErrorMessage(char *message)
{
	int index = 0;

	if (!message)
		return;
	while (message[index] != '\0')
	{
		printErrorCharacter(message[index]);
		index++;
	}
}

/**
 * printErrorCharacter - writes the character c to stderr
 * @character: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int printErrorCharacter(char character)
{
	static int index;
	static char buffer[WRITE_BUFFER_SIZE];

	if (character == BUFFER_FLUSH || index >= WRITE_BUFFER_SIZE)
	{
		write(2, buffer, index);
		index = 0;
	}
	if (character != BUFFER_FLUSH)
		buffer[index++] = character;
	return (1);
}

/**
 * writeToFD - writes the character c to the given file descriptor
 * @character: The character to print
 * @fd: The file descriptor to write to
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int writeToFD(char character, int fd)
{
	static int index;
	static char buffer[WRITE_BUFFER_SIZE];

	if (character == BUFFER_FLUSH || index >= WRITE_BUFFER_SIZE)
	{
		write(fd, buffer, index);
		index = 0;
	}
	if (character != BUFFER_FLUSH)
		buffer[index++] = character;
	return (1);
}

/**
 * printToFD - prints an input string to a given file descriptor
 * @str: the string to be printed
 * @fd: the file descriptor to write to
 *
 * Return: the number of characters put
 */
int printToFD(char *str, int fd)
{
	int count = 0;

	if (!str)
		return (0);
	while (*str)
	{
		count += writeToFD(*str++, fd);
	}
	return (count);
}
