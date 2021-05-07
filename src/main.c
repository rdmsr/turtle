#include <libguile.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <shell/lisp.h>
#include <shell/shell.h>
#include <shell/path.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void parse_args(char *arg)
{
    if (strcmp(arg, "--help") == 0 || strcmp(arg, "-h") == 0)
    {
        printf("Usage: turtle [OPTION]\n");
        printf("-v --version: Shows the current version\n");
        printf("-h --help: Shows this message\n");
    }
    else if (strcmp(arg, "--version") == 0 || strcmp(arg, "-v") == 0)
    {
        printf("Turtle " TURTLE_VERSION "\n");
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, shell_loop);

    scm_init_guile();

    using_history();

    if (argc == 1)
    {
        scm_c_primitive_load_path(path_from_turtle_lib("ansicolors.scm"));
        scm_c_primitive_load(path_from_home(".turtlerc.scm"));

        scm_c_use_module("turtlerc");

        read_history(path_from_home(".turtle_history"));

        shell_loop();
    }
    else
    {
        parse_args(argv[1]);
    }

    return 0;
}
