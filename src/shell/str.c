#include <stdlib.h>
#include <string.h>

void str_replace(char *target, const char *needle, const char *replacement)
{
    char buffer[1024] = {0};
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle), repl_len = strlen(replacement);

    while (1)
    {
        const char *p = strstr(tmp, needle);

        if (p == NULL)
        {
            strcpy(insert_point, tmp);
            break;
        }

        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        tmp = p + needle_len;
    }

    strcpy(target, buffer);
}

char **parse_string(char *string)
{

    char **array = malloc(strlen(string) * sizeof(char *));
    int i = 0;

    array[i] = strtok(string, " \t\r\n\a");

    while (array[i])
    {
        array[++i] = strtok(NULL, " \t\r\n\a");
    }

    return array;
}