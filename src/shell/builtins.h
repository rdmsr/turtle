#ifndef BUILTINS_H
#define BUILTINS_H
#include <stdio.h>
#include <unistd.h>

struct builtin
{
    char *cmd;
    int (*fnc)(char **args);
};

extern struct builtin builtins[];

#endif
