#include <shell/lisp.h>
#include <stdbool.h>

char *lisp_get_var(char *name)
{
    SCM var = scm_c_public_variable("turtlerc", name);

    if (scm_is_false(var))
    {
        return NULL;
    }

    SCM var_ref = scm_variable_ref(var);

    char* text = scm_to_utf8_string(var_ref);
    
    return text;
}
