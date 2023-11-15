#include "shell.h"

int is_chain(info_t *info, char *buf, size_t *p)
{
    size_t pos = *p;

    if (buf[pos] == '|' && buf[pos + 1] == '|')
    {
        buf[pos] = 0;
        pos++;
        info->cmd_buf_type = CMD_OR;
    }
    else if (buf[pos] == '&' && buf[pos + 1] == '&')
    {
        buf[pos] = 0;
        pos++;
        info->cmd_buf_type = CMD_AND;
    }
    else if (buf[pos] == ';')
    {
        buf[pos] = 0;
        info->cmd_buf_type = CMD_CHAIN;
    }
    else
        return (0);
    *p = pos;
    return (1);
}

void check_chain(info_t *info, char *buf, size_t *p, size_t start_pos, size_t length)
{
    size_t pos = *p;

    if (info->cmd_buf_type == CMD_AND)
    {
        if (info->status)
        {
            buf[start_pos] = 0;
            pos = length;
        }
    }
    if (info->cmd_buf_type == CMD_OR)
    {
        if (!info->status)
        {
            buf[start_pos] = 0;
            pos = length;
        }
    }

    *p = pos;
}

int replace_alias(info_t *info)
{
    int i;
    list_t *node;
    char *ptr;

    for (i = 0; i < 10; i++)
    {
        node = node_starts_with(info->alias, info->argv[0], '=');
        if (!node)
            return (0);
        free(info->argv[0]);
        ptr = _strchr(node->str, '=');
        if (!ptr)
            return (0);
        ptr = _strdup(ptr + 1);
        if (!ptr)
            return (0);
        info->argv[0] = ptr;
    }
    return (1);
}

int replace_vars(info_t *info)
{
    int i = 0;
    list_t *node;

    for (i = 0; info->argv[i]; i++)
    {
        if (info->argv[i][0] != '$' || !info->argv[i][1])
            continue;

        if (!_strcmp(info->argv[i], "$?"))
        {
            replace_string(&(info->argv[i]), _strdup(convert_number(info->status, 10, 0)));
            continue;
        }
        if (!_strcmp(info->argv[i], "$$"))
        {
            replace_string(&(info->argv[i]), _strdup(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = node_starts_with(info->env, &info->argv[i][1], '=');
        if (node)
        {
            replace_string(&(info->argv[i]), _strdup(_strchr(node->str, '=') + 1));
            continue;
        }
        replace_string(&info->argv[i], _strdup(""));
    }
    return (0);
}

int replace_string(char **old_str, char *new_str)
{
    free(*old_str);
    *old_str = new_str;
    return (1);
}
