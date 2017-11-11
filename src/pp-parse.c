#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "preprocessor.h"
#include "extern.h"

struct replace_rule *replace_rules;
struct define_rule  *define_rules;

int count_rr, count_dr;   /* replace-rule count, define-rule count */

void preprocess(char *line)
{
    char *dupline, *init;

    dupline = strdup(line);                   /* duplicate line so strtok doesn't mess with main's variable */

    memcpy(dupline,&line[1],strlen(line)-1);  /* advance past the # */

    init = strtok(dupline," ");               /* get the preprocessor directive (e.g. define, replace, etc) */

    if (!strlen(init))                        /* if there were spaces between the % and the directive */
        init = strtok(NULL," ");              /* remove them */

    int directive = directivenum(init);       /* get integer value of directive (see preprocessor.h) */

    if (directive == D_UNDEF) {
        fprintf(stderr,"Warning: unrecognized preprocessor directive %s\n",init);
        free(dupline);
        return;
    }

    char *arg1 = strtok(NULL," ");            /* get first directive string (e.g. %define RULE modification, %replace SEARCH replace, etc) */

    if (arg1[0] == '`') {                     /* defines/searches that need spaces should use `backticks like this` */
        arg1[0] = ' ';
        char *tmp = strtok(NULL,"`");
        strcat(arg1," ");
        strcat(arg1,tmp);
    }

    char *arg2 = strtok(NULL,"\n");           /* get second directive string (e.g. %define rule MODIFICATION, %replace search REPLACE, etc) */

    handle_directive(directive, arg1, arg2);  /* call functions based on directive */

    free(dupline);
}

int directivenum(const char *s)
{
    if (!strcmp(s,"replace"))
        return D_REPLACE;
    else if (!strcmp(s,"defop") || !strcmp(s,"defsyn"))
        return D_DEFINE;
    else if (!strcmp(s,"rule"))
        return D_RULE;
    else
        return D_UNDEF;
}

void handle_directive(int directive, const char *arg1, const char *arg2)
{
    switch (directive) {
      case D_REPLACE:
        add_replace_rule(arg1,arg2);
        break;
      case D_DEFINE:
        add_define_rule(arg1,arg2);
        break;
      case D_RULE:
        modify_ecpp_rules(arg1,arg2);
        break;
    }
}

void add_replace_rule(const char *arg1, const char *arg2)
{
    replace_rules = realloc(replace_rules,(count_rr + 1) * sizeof(struct replace_rule));  /* make space for the new rule */

    replace_rules[count_rr].search = strdup(arg1);
    replace_rules[count_rr].replace = strdup(arg2);
    count_rr++;
}

void add_define_rule(const char *arg1, const char *arg2)
{
    define_rules = realloc(define_rules,(count_dr + 1) * sizeof(struct replace_rule));    /* make space for the new rule */

    FILE *search = fopen(".ecpp.tmp1","w+");    /* if fmemopen were standard this would be much easier ._. */

    FILE *replace = fopen(".ecpp.tmp2","w+");

    fprintf(search,"%s",arg1);
    fprintf(replace,"%s",arg2);

    rewind(search);
    rewind(replace);

    define_rules[count_dr].search = tokenize(search);
    define_rules[count_dr].replace = tokenize(replace);

    IF_DEBUG(printf("--- define rule %d search tokens (%s) ---\n",count_dr,arg1));
    IF_DEBUG(print_list(define_rules[count_dr].search));
    IF_DEBUG(printf("--- define rule %d replace tokens (%s) ---\n",count_dr,arg2));
    IF_DEBUG(print_list(define_rules[count_dr].replace));

    fclose(search);
    fclose(replace);
    remove(".ecpp.tmp1");
    remove(".ecpp.tmp2");

    count_dr++;
}