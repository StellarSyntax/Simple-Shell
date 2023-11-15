#include "shell.h"

size_t linked_list_length(const list_t *ptr)
{
    size_t count = 0;

    while (ptr)
    {
        ptr = ptr->next;
        count++;
    }

    return (count);
}

char **list_to_array(list_t *start)
{
    list_t *current = start;
    size_t len = linked_list_length(start), index;
    char **strings;
    char *string;

    if (!start || !len)
        return (NULL);

    strings = malloc(sizeof(char *) * (len + 1));
    if (!strings)
        return (NULL);

    for (index = 0; current; current = current->next, index++)
    {
        string = malloc(_strlen(current->str) + 1);
        if (!string)
        {
            for (size_t j = 0; j < index; j++)
                free(strings[j]);
            free(strings);
            return (NULL);
        }

        string = _strcpy(string, current->str);
        strings[index] = string;
    }

    strings[index] = NULL;
    return (strings);
}

size_t print_linked_list(const list_t *ptr)
{
    size_t count = 0;

    while (ptr)
    {
        _puts(convert_number(ptr->num, 10, 0));
        _putchar(':');
        _putchar(' ');
        _puts(ptr->str ? ptr->str : "(nil)");
        _puts("\n");
        ptr = ptr->next;
        count++;
    }

    return (count);
}

list_t *find_node_starting_with(list_t *ptr, char *prefix, char next_char)
{
    char *match = NULL;

    while (ptr)
    {
        match = starts_with(ptr->str, prefix);
        if (match && ((next_char == -1) || (*match == next_char)))
            return (ptr);
        ptr = ptr->next;
    }

    return (NULL);
}

ssize_t get_node_position(list_t *start, list_t *target_node)
{
    size_t index = 0;

    while (start)
    {
        if (start == target_node)
            return (index);
        start = start->next;
        index++;
    }

    return (-1);
}
