#define _DEFAULT_SOURCE
#include <shell/lisp.h>
#include <shell/str.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void prompt_parse(char *str, char *format)
{
    int position = 0;

    while (*format)
    {

        if (*format == '%')
        {
            format++;
            switch (*format)
            {
            case 'u':
                strcat(str, getenv("USER"));
                position += strlen(getenv("USER"));
                break;

            case 'd':
            {
                char cwd[256];
                getcwd(cwd, sizeof(cwd));
                strcat(str, cwd);

                if (strstr(str, getenv("HOME")))
                {

                    str_replace(str, getenv("HOME"), "~");
                    position += strlen(cwd) - (strlen(getenv("HOME")) - 1);
                }
                else
                    position += strlen(cwd);

                break;
            }

            case 'h':
            {
                char hostname[1024];
                gethostname(hostname, 1023);
                strcat(str, hostname);
                position += strlen(hostname);
                break;
            }

            default:
                str[position] = '%';
                position++;
                break;
            }
        }

        else
        {
            str[position] = *format;
            position++;
        }

        format++;
    }
}

char *prompt(int status)
{

    static char p_prompt[4096];
    char *prompt = NULL;

    memset(p_prompt, '\0', 4096);

    /* Take the scheme prompt variable if set */
    if (lisp_get_var("prompt"))
    {
        prompt = lisp_get_var("prompt");

        prompt_parse(p_prompt, prompt);
    }

    /* Use a default prompt */
    else
    {
        prompt = "λ ";

        strcpy(p_prompt, prompt);
    }

    if (status != 0)
    {
        if (lisp_get_var("failed-prompt"))
        {
            prompt = lisp_get_var("failed-prompt");
            memset(p_prompt, 0, strlen(p_prompt));
            prompt_parse(p_prompt, prompt);
        }
    }
    else
    {
        /* Take the scheme prompt variable if set */
        if (lisp_get_var("prompt"))
        {
            prompt = lisp_get_var("prompt");
            memset(p_prompt, 0, strlen(p_prompt));
            prompt_parse(p_prompt, prompt);
        }

        /* Use a default prompt */
        else
        {

            prompt = "λ ";
            strcpy(p_prompt, prompt);
        }
    }

    return p_prompt;
}
