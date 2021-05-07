#define _DEFAULT_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <libguile.h>
#include <shell/builtins.h>
#include <shell/lisp.h>
#include <shell/shell.h>

static bool failed = false;

static char **parse_string(char *string)
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

static int spawn_command(char *command)
{

    char buffer[strlen(command)];

    strcpy(buffer, command);

    char **command_array = NULL;

    command_array = parse_string(buffer);

    pid_t pid;

    int status = 0;
    pid = fork();

    /* Child process */
    if (pid == 0)
    {

        if (execvp(command_array[0], command_array) == -1)
        {

            perror("turtle");
        }

        exit(EXIT_FAILURE);
    }

    else if (pid < 0)
    {
        /* Error forking */
        perror("turtle");
    }

    else
    {
        do
        {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    if (status > 0)
    {
        failed = true;
    }

    else
        failed = false;

    return 1;
}

static int execute_command(char *command)
{

    if (command)
    {
        int i;

        char buffer[strlen(command)];

        strcpy(buffer, command);

        char **command_array = NULL;

        command_array = parse_string(buffer);

        for (i = 0; builtins[i].cmd != NULL; i++)
        {
            if (strcmp(command_array[0], builtins[i].cmd) == 0)
            {
                return builtins[i].fnc(command_array);
            }
        }

        return spawn_command(command);
    }

    return -1;
}

static void str_replace(char *target, const char *needle, const char *replacement)
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

static void parse_prompt(char *str, char *format)
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

void make_prompt()
{

    char *command = NULL, *prompt, p_prompt[4096] = {0};
    char hist_file[1024];
    char *temp = getenv("HOME");

    strcpy(hist_file, temp);
    strcat(hist_file, "/.turtle_history");

    read_history(hist_file);

    /* Take the scheme prompt variable if set */
    if (lisp_get_var("prompt"))
    {
        prompt = lisp_get_var("prompt");

        parse_prompt(p_prompt, prompt);
    }

    /* Use a default prompt */
    else
    {
        prompt = "λ ";

        strcpy(p_prompt, prompt);
    }

    do
    {

        if (write_history(hist_file) != 0)
            perror("turtle");

        if (failed)
        {
            if (lisp_get_var("failed-prompt"))
            {
                prompt = lisp_get_var("failed-prompt");
                memset(p_prompt, 0, strlen(p_prompt));
                parse_prompt(p_prompt, prompt);
            }
        }
        else
        {
            /* Take the scheme prompt variable if set */
            if (lisp_get_var("prompt"))
            {
                prompt = lisp_get_var("prompt");
                memset(p_prompt, 0, strlen(p_prompt));
                parse_prompt(p_prompt, prompt);
            }

            /* Use a default prompt */
            else
            {

                prompt = "λ ";
                strcpy(p_prompt, prompt);
            }
        }

        /* Read the next command to execute */
        command = readline(p_prompt);
        if (command == NULL)
        {
            return; /* exit on C-d */
        }

        if (*command == '\0')
        {
            /* ignore blank command */
            continue;
        }

        if (command[0] != '(')
        {
            /* Add to history */
            add_history(command);

            /* Execute it */
            execute_command(command);

            free(command);
        }
        else
        {
            add_history(command);
            scm_c_eval_string(command);

            free(command);
        }

    } while (1);
}
