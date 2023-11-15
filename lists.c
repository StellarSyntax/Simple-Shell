#include "shell.h"

list_t *add_to_start(list_t **init, const char *str, int num)
{
    list_t *new_start;

    if (!init)
        return (NULL);

    new_start = malloc(sizeof(list_t));
    if (!new_start)
        return (NULL);

    _memset((void *)new_start, 0, sizeof(list_t));
    new_start->num = num;

    if (str)
    {
        new_start->str = _strdup(str);
        if (!new_start->str)
        {
            free(new_start);
            return (NULL);
        }
    }

    new_start->next = *init;
    *init = new_start;
    return (new_start);
}

list_t *add_to_end(list_t **init, const char *str, int num)
{
    list_t *new_node, *current_node;

    if (!init)
        return (NULL);

    current_node = *init;
    new_node = malloc(sizeof(list_t));
    if (!new_node)
        return (NULL);

    _memset((void *)new_node, 0, sizeof(list_t));
    new_node->num = num;

    if (str)
    {
        new_node->str = _strdup(str);
        if (!new_node->str)
        {
            free(new_node);
            return (NULL);
        }
    }

    if (current_node)
    {
        while (current_node->next)
            current_node = current_node->next;
        current_node->next = new_node;
    }
    else
        *init = new_node;

    return (new_node);
}

size_t print_strings(const list_t *ptr)
{
    size_t count = 0;

    while (ptr)
    {
        _puts(ptr->str ? ptr->str : "(nil)");
        _puts("\n");
        ptr = ptr->next;
        count++;
    }

    return (count);
}

int delete_node(list_t **init, unsigned int index)
{
    list_t *node, *prev_node;
    unsigned int i = 0;

    if (!init || !*init)
        return (0);

    if (!index)
    {
        node = *init;
        *init = (*init)->next;
        free(node->str);
        free(node);
        return (1);
    }

    node = *init;

    while (node)
    {
        if (i == index)
        {
            prev_node->next = node->next;
            free(node->str);
            free(node);
            return (1);
        }

        i++;
        prev_node = node;
        node = node->next;
    }

    return (0);
}

void free_linked_list(list_t **init)
{
    list_t *node, *next_node, *start;

    if (!init || !*init)
        return;

    start = *init;
    node = start;

    while (node)
    {
        next_node = node->next;
        free(node->str);
        free(node);
        node = next_node;
    }

    *init = NULL;
}
