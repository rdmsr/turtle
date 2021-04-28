#include <libguile.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <shell/lisp.h>
#include <shell/shell.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    scm_init_guile();

    char rc_file[1024];
    char *temp = getenv("HOME");
    
    strcpy(rc_file, temp);
    strcat(rc_file, "/.turtlerc.scm");

    scm_c_primitive_load(rc_file);

    scm_c_use_module("turtlerc");

    using_history();

    if (argc == 1)
    {
        prompt();
    }
    else
        parse_args(argv[1]);

    return 0;
}
