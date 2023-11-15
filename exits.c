#include "shell.h"

char *_string_copy_n(char *destination, char *source, int max_chars)
{
    int i, j;
    char *starting_point = destination;

    i = 0;
    while (source[i] != '\0' && i < max_chars - 1)
    {
        destination[i] = source[i];
        i++;
    }
    if (i < max_chars)
    {
        j = i;
        while (j < max_chars)
        {
            destination[j] = '\0';
            j++;
        }
    }
    return (starting_point);
}

char *_string_concat_n(char *destination, char *source, int max_bytes)
{
    int i, j;
    char *starting_point = destination;

    i = 0;
    j = 0;
    while (destination[i] != '\0')
        i++;
    while (source[j] != '\0' && j < max_bytes)
    {
        destination[i] = source[j];
        i++;
        j++;
    }
    if (j < max_bytes)
        destination[i] = '\0';
    return (starting_point);
}

char *_string_find_char(char *string, char character)
{
    do
    {
        if (*string == character)
            return (string);
    } while (*string++ != '\0');

    return (NULL);
}
     
