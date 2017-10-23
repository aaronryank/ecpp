#include <string.h>
#include "string.h"

int str_eq(const char *s, const char **in, int inlen)
{
    int i;
    for (i = 0; i < inlen; i++)
        if (!strcmp(s,in[i]))
            return 1;
    return 0;
}

int str_only_contains(const char *s, const char *bytes)
{
    int i;
    for (i = 0; s[i]; i++)
        if (!strchr(bytes,s[i]))
            return 0;
    return 1;
}

int str_contains(const char *s, const char *bytes)
{
    int i;
    for (i = 0; s[i]; i++)
        if (strchr(bytes,s[i]))
            return 1;
    return 0;
}