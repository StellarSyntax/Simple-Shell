#include "shell.h"

/**
 * checkInteractiveMode - determines if the shell is in interactive mode
 * @info: pointer to the information structure
 *
 * Return: 1 if in interactive mode, 0 otherwise
 */
int checkInteractiveMode(info_t *info)
{
	return (isatty(STDIN_FILENO) && info->readfd <= 2);
}

/**
 * isDelimiter - checks if a character is a delimiter
 * @character: the character to check
 * @delimiters: the delimiter string
 *
 * Return: 1 if true, 0 if false
 */
int isDelimiter(char character, char *delimiters)
{
	while (*delimiters)
		if (*delimiters++ == character)
			return (1);
	return (0);
}

/**
 * isAlphabetic - checks for an alphabetic character
 * @ch: The character to check
 *
 * Return: 1 if ch is alphabetic, 0 otherwise
 */
int isAlphabetic(int ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 * stringToInteger - converts a string to an integer
 * @str: the string to be converted
 *
 * Return: 0 if no numbers in the string, the converted number otherwise
 */
int stringToInteger(char *str)
{
	int index, sign = 1, flag = 0, output;
	unsigned int result = 0;

	for (index = 0;  str[index] != '\0' && flag != 2; index++)
	{
		if (str[index] == '-')
			sign *= -1;

		if (str[index] >= '0' && str[index] <= '9')
		{
			flag = 1;
			result *= 10;
			result += (str[index] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		output = -result;
	else
		output = result;

	return (output);
}
