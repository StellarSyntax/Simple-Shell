#include "shell.h"

/**
 * convertStringToInt - converts a string to an integer
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int convertStringToInt(char *s)
{
	int index = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;

	for (index = 0; s[index] != '\0'; index++)
	{
		if (s[index] >= '0' && s[index] <= '9')
		{
			result *= 10;
			result += (s[index] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}

	return (result);
}

/**
 * printErrorMessage - prints an error message
 * @info: the parameter & return info struct
 * @errorType: string containing specified error type
 * Return: Nothing
 */
void printErrorMessage(info_t *info, char *errorType)
{
	printToFD(info->fname, STDERR_FILENO);
	printToFD(": ", STDERR_FILENO);
	printDecimal(info->line_count, STDERR_FILENO);
	printToFD(": ", STDERR_FILENO);
	printToFD(info->argv[0], STDERR_FILENO);
	printToFD(": ", STDERR_FILENO);
	printToFD(errorType);
}

/**
 * printDecimal - prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the filedescriptor to write to
 *
 * Return: number of characters printed
 */
int printDecimal(int input, int fd)
{
	int (*printChar)(char) = printCharToFD;
	int i, count = 0;
	unsigned int absoluteValue, current;

	if (fd == STDERR_FILENO)
		printChar = printCharToFDError;

	if (input < 0)
	{
		absoluteValue = -input;
		printChar('-');
		count++;
	}
	else
		absoluteValue = input;

	current = absoluteValue;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (absoluteValue / i)
		{
			printChar('0' + current / i);
			count++;
		}
		current %= i;
	}
	printChar('0' + current);
	count++;

	return (count);
}

/**
 * convertNumberToString - converter function, a clone of itoa
 * @num: number
 * @base: base
 * @flags: argument flags
 *
 * Return: string
 */
char *convertNumberToString(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';
	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do {
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * removeComments - function replaces the first instance of '#' with '\0'
 * @buf: address of the string to modify
 *
 * Return: Always 0;
 */
void removeComments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}
