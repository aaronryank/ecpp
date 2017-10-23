#ifndef _ECPP_PREPROCESSOR_H
#define _ECPP_PREPROCESSOR_H

#include "token.h"
#define absolute(x) (x<0?-x:x)

enum {
  D_UNDEF,
  D_REPLACE,
  D_DEFINE,
  D_RULE
} directive_types;

struct replace_rule {
  char *search, *replace;
};

struct define_rule {
  token_t *search, *replace;
};

struct _varnames {
  char *rule;  // rule's variable name
  char *src;   // actual variable in src
};

#endif // ecpp's preprocessor.h