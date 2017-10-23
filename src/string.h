#ifndef _ECPP_STRING_H
#define _ECPP_STRING_H

#define LOWERCASE_ALPHA "abcdefghijklmnopqrstuvwxyz"
#define UPPERCASE_ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

extern int str_eq(const char *, const char **, int);
extern int str_only_contains(const char *, const char *);
extern int str_contains(const char *, const char *);

#endif
