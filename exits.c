#include "shell.h"

/**
 * copyString - copies a string
 * @destination: the destination string to be copied to
 * @source: the source string
 * @limit: the maximum number of characters to be copied
 * Return: the concatenated string
 */
char *copyString(char *destination, char *source, int limit)
{
	int indexSrc, indexDest;
	char *result = destination;

	indexSrc = 0;
	while (source[indexSrc] != '\0' && indexSrc < limit - 1)
	{
		destination[indexSrc] = source[indexSrc];
		indexSrc++;
	}
	if (indexSrc < limit)
	{
		indexDest = indexSrc;
		while (indexDest < limit)
		{
			destination[indexDest] = '\0';
			indexDest++;
		}
	}
	return result;
}

/**
 * concatenateStrings - concatenates two strings
 * @first: the first string
 * @second: the second string
 * @limit: the maximum number of bytes to be used
 * Return: the concatenated string
 */
char *concatenateStrings(char *first, char *second, int limit)
{
	int indexFirst, indexSecond;
	char *result = first;

	indexFirst = 0;
	while (first[indexFirst] != '\0')
		indexFirst++;
	indexSecond = 0;
	while (second[indexSecond] != '\0' && indexSecond < limit)
	{
		first[indexFirst] = second[indexSecond];
		indexFirst++;
		indexSecond++;
	}
	if (indexSecond < limit)
		first[indexFirst] = '\0';

	return result;
}

/**
 * findCharacter - locates a character in a string
 * @str: the string to be parsed
 * @ch: the character to look for
 * Return: (str) a pointer to the memory area str
 */
char *findCharacter(char *str, char ch)
{
	do {
		if (*str == ch)
			return str;
	} while (*str++ != '\0');

	return NULL;
}
