#include <stdio.h>
#include "extern.h"

short int if_directive(const char *arg1, const char *arg2, token_t *tokens)
{
    if (!strcmp(arg1,"func_exist"))
        return func_exist(arg2,tokens);
    else if (!strcmp(arg1,"func_not_exist") || !strcmp(arg1,"!func_exist"))
        return !func_exist(arg2,tokens);
}

short int func_exist(const char *funcname, token_t *_tokens)
{
    token_t *tokens = _tokens;

    while (tokens->prev)
        tokens = tokens->prev;

    while (tokens->next) {
        if (tokens->prev->type == TYPE_RESERVED && !strcmp(tokens->val,funcname))
            return 1;
        tokens = tokens->next;
    }

    return 0;
}
