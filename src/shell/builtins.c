#include <shell/builtins.h>
#include <unistd.h>
#include <stdlib.h>

static char *builtins[] = {
    "cd",
    "help",
    "exit"};

static int num_builtins()
{
    return sizeof(builtins) / sizeof(char *);
}

int builtin_cd(char **args)
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
