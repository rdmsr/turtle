#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

char *path_from_home(const char *name)
{
    static char file_path[PATH_MAX];
    char *home_path;

    memset(file_path, 0, PATH_MAX);
    home_path = getenv("HOME");
    if (home_path)
    {
        strcpy(file_path, home_path);
        strcat(file_path, "/");
    }
    strcat(file_path, name);

    return file_path;
}

char *path_from_turtle_lib(const char *name)
{
    char *file_path;

    file_path = path_from_home(".local/share/turtle/lib/");
    strcat(file_path, name);

    return file_path;
}