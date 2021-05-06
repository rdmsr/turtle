#include <libguile.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <shell/lisp.h>
#include <shell/shell.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char *get_lib_file(char *name)
{
    static char file[PATH_MAX];
    char *temp = getenv("HOME");

    memset(file, 0, PATH_MAX);
    strcpy(file, temp);
    strcat(file, "/.local/share/turtle/lib/");
    strcat(file, name);

    return file;
}

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
    char rc_file[PATH_MAX];
    char *temp = getenv("HOME");

    scm_init_guile();

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
    {
        parse_args(argv[1]);
    }

    return 0;
}
