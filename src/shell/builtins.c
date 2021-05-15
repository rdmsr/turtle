#include <shell/builtins.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libguile.h>
#include <shell/path.h>

static int builtin_cd(char **args)
{
    if (!args[1])
    {
        chdir(getenv("HOME"));
    }

    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("turtle");
            return 1;
        }
    }

    return 0;
}

static int builtin_help()
{
    int i;

    printf("List of builtins commands: \n");
    for (i = 0; builtins[i].cmd != NULL; i++)
    {
        printf("  %s\n", builtins[i].cmd);
    }
    return 0;
}

static int builtin_exit(char **args)
{
    int status = 0;

    if (args[1])
    {
        status = atoi(args[1]);
    }
    exit(status);

    return 0;
}

struct builtin builtins[] = {
    {"cd", builtin_cd},
    {"help", builtin_help},
    {"exit", builtin_exit},
    {NULL, NULL}};
