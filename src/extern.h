#ifndef _ECPP_EXTERN_H
#define _ECPP_EXTERN_H

#include <stdio.h>
#include "token.h"
#include "pretty.h"

/* main.c */

extern int flags[127];

extern char *parse_args(int, char **);
extern void  read_rules(FILE *);

#define IF_DEBUG(x) flags['d'] ? x : 0;

/* tokenizer.c */

extern token_t  *tokenize(FILE *);
extern int       gettype(const char *);
extern short int compatibletypes(const char *, int);

/* pp-setup.c */

extern struct replace_rule *replace_rules;
extern struct define_rule  *define_rules;
extern int                  count_rr;
extern int                  count_dr;

extern void preprocess(char *);
extern int  directivenum(const char *);
extern void handle_directive(int, const char *, const char *);
extern void add_replace_rule(const char *, const char *);
extern void add_define_rule(const char *, const char *);

/* preprocessor.c */

extern token_t          *preprocess_line(int, token_t *);
extern int               matches_replace_rule(const char *);
extern short int         compatible_defreplace_types(const token_t *, const token_t *);
extern short int         _matches_define_rule(token_t *, token_t *, int *);
extern int               matches_define_rule(token_t *, int *);
extern void              do_replace(token_t **, int);
extern int               count_tokens(token_t *);
extern struct _varnames *get_variable_names(token_t *, token_t *, int);
extern char             *getvarname(const char *, struct _varnames *, int);
extern void              do_replace_def(token_t **, int, int, int);

/* rules.c */

extern const char *typenames[];
extern const char *DEFAULT_RESERVED_KEYWORDS[];
extern int         reserved_keyword_count;
extern const char *DEFAULT_CONTROL_FLOW_KEYWORDS[];
extern int         control_flow_keyword_count;
extern char      **RESERVED_KEYWORDS;
extern char      **CONTROL_FLOW_KEYWORDS;

extern void setup_rules(void);
extern void modify_ecpp_rules(const char *, const char *);
extern void add_control_flow_keyword(const char *);
extern void add_reserved_keyword(const char *);

/* pretty.c */

extern void prettyprint_line(int, token_t *, struct prettyprint *);

/* uncomment.c */

extern void uncomment(const char *, const char *);

#endif // ecpp's extern.h
