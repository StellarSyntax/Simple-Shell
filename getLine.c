
#include "shell.h"

ssize_t input_buffer(info_t *info, char **buffer, size_t *length)
{
    ssize_t bytes_read = 0;
    size_t length_position = 0;

    if (!*length) 
    {
        free(*buffer);
        *buffer = NULL;
        signal(SIGINT, handleSIGINT);
#if USE_GETLINE
        bytes_read = getline(buffer, &length_position, stdin);
#else
        bytes_read = custom_getline(info, buffer, &length_position);
#endif
        if (bytes_read > 0)
        {
            if ((*buffer)[bytes_read - 1] == '\n')
            {
                (*buffer)[bytes_read - 1] = '\0';
                bytes_read--;
            }
            info->line_count_flag = 1;
            remove_comments(*buffer);
            build_history(info, *buffer, info->history_count++);
            {
                *length = bytes_read;
                info->command_buffer = buffer;
            }
        }
    }
    return (bytes_read);
}

ssize_t get_user_input(info_t *info)
{
    static char *buffer;
    static size_t iterator_i, iterator_j, buffer_length;
    ssize_t bytes_read = 0;
    char **buffer_pointer = &(info->arguments), *ptr;

    _putchar(FLUSH_BUFFER);
    bytes_read = input_buffer(info, &buffer, &buffer_length);
    if (bytes_read == -1) 
        return (-1);
    if (buffer_length) 
    {
        iterator_j = iterator_i;
        ptr = buffer + iterator_i;

        check_command_chain(info, buffer, &iterator_j, iterator_i, buffer_length);
        while (iterator_j < buffer_length) 
        {
            if (is_command_chain(info, buffer, &iterator_j))
                break;
            iterator_j++;
        }

        iterator_i = iterator_j + 1;
        if (iterator_i >= buffer_length) 
        {
            iterator_i = buffer_length = 0;
            info->cmd_buf_type = NORMAL_COMMAND;
        }

        *buffer_pointer = ptr;
        return (_strlen(ptr));
    }

    *buffer_pointer = buffer;
    return (bytes_read);
}

ssize_t read_buffer(info_t *info, char *buf, size_t *size)
{
    ssize_t bytes_read = 0;

    if (*size)
        return (0);
    bytes_read = read(info->read_fd, buf, READ_BUFFER_SIZE);
    if (bytes_read >= 0)
        *size = bytes_read;
    return (bytes_read);
}

int custom_getline(info_t *info, char **ptr, size_t *length)
{
    static char buffer[READ_BUFFER_SIZE];
    static size_t iterator_i, buffer_length;
    size_t k;
    ssize_t bytes_read = 0, size = 0;
    char *pointer = NULL, *new_pointer = NULL, *c;

    pointer = *ptr;
    if (pointer && length)
        size = *length;
    if (iterator_i == buffer_length)
        iterator_i = buffer_length = 0;

    bytes_read = read_buffer(info, buffer, &buffer_length);
    if (bytes_read == -1 || (bytes_read == 0 && buffer_length == 0))
        return (-1);

    c = _strchr(buffer + iterator_i, '\n');
    k = c ? 1 + (unsigned int)(c - buffer) : buffer_length;
    new_pointer = _realloc(pointer, size, size ? size + k : k + 1);
    if (!new_pointer) 
        return (pointer ? free(pointer), -1 : -1);

    if (size)
        _strncat(new_pointer, buffer + iterator_i, k - iterator_i);
    else
        _strncpy(new_pointer, buffer + iterator_i, k - iterator_i + 1);

    size += k - iterator_i;
    iterator_i = k;
    pointer = new_pointer;

    if (length)
        *length = size;
    *ptr = pointer;
    return (size);
}

void handleSIGINT(__attribute__((unused)) int signal_number)
{
    _puts("\n");
    _puts("$ ");
    _putchar(FLUSH_BUFFER);
}
