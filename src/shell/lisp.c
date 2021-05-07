#include <stdlib.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <libguile.h>
#pragma GCC diagnostic pop

char *lisp_get_var(char *name)
{
    SCM var;
    SCM var_ref;
    char* text;

    var = scm_c_public_variable("turtlerc", name);

    if (scm_is_false(var))
    {
        return NULL;
    }
    var_ref = scm_variable_ref(var);
    text = scm_to_utf8_string(var_ref);
    
    return text;
}
