#define inline
#include <libguile.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <shell/lisp.h>
#include <shell/shell.h>
#include <stdio.h>
#include <stdlib.h>

char *get_lib_file(char *name)
{
    char file[1024];
    char *temp = getenv("HOME");

    strcpy(file, temp);
    strcat(file, "/.local/share/turtle/lib/");
    strcat(file, name);

    char *ret = malloc(sizeof(char*) * strlen(temp));
    strcpy(ret, file);

    return ret;
}

int main(int argc, char *argv[])
{

    scm_init_guile();

    char rc_file[1024];
    char *temp = getenv("HOME");

    strcpy(rc_file, temp);
    strcat(rc_file, "/.turtlerc.scm");

    using_history();

    if (argc == 1)
    {

        scm_c_primitive_load_path(get_lib_file("ansicolors.scm"));
        scm_c_primitive_load(rc_file);

        scm_c_use_module("turtlerc");

        make_prompt();
    }
    else
        parse_args(argv[1]);

    return 0;
}
