#include <libguile.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <shell/builtins.h>
#include <shell/lisp.h>
#include <shell/prompt.h>
#include <shell/shell.h>
#include <shell/str.h>
#include <shell/path.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int spawn_command(char **command_array)
{
    pid_t pid;
    int status;

    status = 0;
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

    return status;
}

static int execute_command(char *command)
{
    int i;
    int ret_status;
    char **command_array = NULL;

    ret_status = -1;
    if (command)
    {
        command_array = parse_string(command);

        for (i = 0; builtins[i].cmd != NULL; i++)
        {
            if (strcmp(command_array[0], builtins[i].cmd) == 0)
            {
                ret_status = builtins[i].fnc(command_array);
                free(command_array);
                return ret_status;
            }
        }

        ret_status = spawn_command(command_array);
        free(command_array);
    }

    return ret_status;
}

void shell_loop(void)
{
    int ret_status = 0;
    char *command = NULL;

    do
    {
        if (write_history(path_from_home(".turtle_history")) != 0)
        {
            perror("turtle");
        }

        /* Read the next command to execute */
        command = readline(prompt(ret_status));
	
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
            ret_status = execute_command(command);

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
