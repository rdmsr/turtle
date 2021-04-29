
#define _DEFAULT_SOURCE
#include <stdio.h>

#include <readline/history.h>
#include <readline/readline.h>
#include <shell/lisp.h>
#include <shell/shell.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int builtin_cd(char **args);
int builtin_help(char **args);
int builtin_exit(char **args);

char *builtins[] = {
    "cd",
    "help",
    "exit"};

bool failed = false;
int (*builtin_funcs[])(char **) = {&builtin_cd, &builtin_help, &builtin_exit};

int num_builtins()
{
    return sizeof(builtins) / sizeof(char *);
}

int builtin_cd(char **args)
{
    if (!args[1])
    {
        fprintf(stderr, "turtle: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("turtle");
        }
    }
    return 1;
}

int builtin_help(char **args)
{
    int i;

    printf("List of builtins commands: \n");
    for (i = 0; i < num_builtins(); i++)
    {
        printf("  %s\n", builtins[i]);
    }

    (void)(args);
    return 1;
}

int builtin_exit(char **args)
{

    (void)(args);
    return 0;
}

void parse_args(char *arg)
{
    if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0)
    {
        printf("Usage: turtle [OPTION]\n");
        printf("-v --version: Shows the current version\n");
        printf("-h --help: Shows this message\n");
    }

    if (strcmp(arg, "--version") == 0 || strcmp(arg, "-v") == 0)
    {
        printf("Turtle V0.0.15\n");
    }
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

int spawn_command(char *command)
{

    char buffer[strlen(command)];

    strcpy(buffer, command);

    char **command_array = NULL;

    command_array = parse_string(buffer);

    pid_t pid, wait_pid;

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
            wait_pid = waitpid(pid, &status, WUNTRACED);
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

int execute_command(char *command)
{

    if (command)
    {
        int i;

        char buffer[strlen(command)];

        strcpy(buffer, command);

        char **command_array = NULL;

        command_array = parse_string(buffer);

        for (i = 0; i < num_builtins(); i++)
        {
            if (strcmp(command_array[0], builtins[i]) == 0)
            {
                return (*builtin_funcs[i])(command_array);
            }
        }

        return spawn_command(command);
    }

    return -1;
}

void str_replace(char *target, const char *needle, const char *replacement)
{
    char buffer[1024] = {0};
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

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

void parse_prompt(char *str, char *format)
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
    int status;
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

        if (command && *command && command[0] != '(')
        {
            /* Add to history */
            add_history(command);

            /* Execute it */
            status = execute_command(command);

            free(command);
        }

        else if (command[0] == '(')
        {
            scm_c_eval_string(command);
        }

    } while (status);
}
