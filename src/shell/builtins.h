#ifndef BUILTINS_H
#define BUILTINS_H
#include <stdio.h>
#include <unistd.h>

int builtin_cd(char **args);
int builtin_help(char **args);
int builtin_exit(char **args);

#endif
