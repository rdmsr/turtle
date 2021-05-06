#ifndef TURTLE_BUILTINS_H
#define TURTLE_BUILTINS_H 1

struct builtin
{
    char *cmd;
    int (*fnc)(char **args);
};

extern struct builtin builtins[];

#endif /* !TURTLE_BUILTINS_H */
