#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "preprocessor.h"
#include "token.h"
#include "types.h"
#include "extern.h"

token_t *preprocess_line(int line, token_t *tokens)
{
    if (!tokens)
        return NULL;

    while (tokens->next && tokens->line < line)
        tokens = tokens->next;

    /* tokens->next so the last line of this loop doesn't irreversably set tokens to NULL */

    while (tokens->next && tokens->line == line)
    {
        int result, count = 0;

        if ((result = matches_replace_rule(tokens->val)) >= 0) {
            IF_DEBUG(printf("MATCHES REPLACE- idx = %d\n",result));
            do_replace(&tokens,result);
        }
        if ((result = matches_define_rule(tokens,&count)) >= 0) {
            IF_DEBUG(printf("MATCHES DEFINE- idx = %d, count = %d\n",result,count));
            do_replace_def(&tokens,result,count,line);
        }

        tokens = tokens->next;
    }

    rewind_token(&tokens);
    return tokens;
}

int matches_replace_rule(const char *s)
{
    int i;
    for (i = 0; i < count_rr; i++)
        if (!strcmp(s,replace_rules[i].search))
            return i;
    return -1;
}

short int compatible_defreplace_types(const token_t *t1, const token_t *t2)
{
    if (t1->type == TYPE_KEYWORD && t2->type == TYPE_LITERAL)
        return 1;

    if (t1->type == TYPE_OPERATOR && t2->type == TYPE_OPERATOR)
        return !strcmp(t1->val,t2->val);

    return t1->type == t2->type;
}

short int _matches_define_rule(token_t *_check, token_t *_base, int *count)
{
    token_t *check = _check;
    token_t *base = _base;
    for (*count = 0; base && check; (*count)++, base = base->next, check = check->next)
        if (!compatible_defreplace_types(base,check))
            return 0;
    return 1;
}

int matches_define_rule(token_t *_t, int *count)
{
    token_t *t = _t;
    int i;
    for (i = 0; i < count_dr; i++)
        if (_matches_define_rule(t,define_rules[i].search,count))
            return i;
    return -1;
}

void do_replace(token_t **t, int i)
{
    free((*t)->val);
    (*t)->val = strdup(replace_rules[i].replace);
    (*t)->type = gettype((*t)->val);
}

int count_tokens(token_t *_t)
{
    token_t *t = _t;
    int i;
    for (i = 1; t->next; i++, t = t->next)
        ;
    return i;
}

struct _varnames *get_variable_names(token_t *_src, token_t *_rule, int size)
{
    struct _varnames *varnames = malloc(sizeof(struct _varnames) * size);
    memset(varnames,0,sizeof(struct _varnames) * size);
    token_t *src  = _src;
    token_t *rule = _rule;

    int i, v;
    for (i = v = 0; i < size; i++) {
        if (rule->type == TYPE_KEYWORD && (src->type == TYPE_KEYWORD || src->type == TYPE_LITERAL)) {
            varnames[v].src  = strdup(src->val);
            varnames[v].rule = strdup(rule->val);
            v++;
        }
        rule = rule->next;
        src  = src->next;
    }

    return varnames;
}

char *getvarname(const char *rule, struct _varnames *varnames_1, int count)
{
    struct _varnames *varnames = varnames_1;   /* prevent original pointer advancement */


    int i;
    for (i = 0; i < count; i++)
        if (varnames[i].rule && !strcmp(rule,varnames[i].rule))
            return varnames[i].src;

    return NULL;
}

/* src: foo := 3 */

/* rule(search): x := 3 */

/* rule(replace): x = 3 */


void do_replace_def(token_t **t, int idx, int count_src, int line)
{
    int count_rule = count_tokens(define_rules[idx].replace);
    IF_DEBUG(printf("tokens in source/directive: %d\ntokens in replacement: %d\n",count_src,count_rule));

    struct _varnames *varnames;
    varnames = get_variable_names(*t,define_rules[idx].search,count_src);

    int i;
    if (flags['d'])
        for (i = 0; i < count_src; i++)
            if (varnames[i].src && varnames[i].rule)
                printf("source token %s matches rule token %s\n",varnames[i].src,varnames[i].rule);

    int l = (*t)->line;

    if (count_src < count_rule) {                    /* if the rule has less tokens than the search-matching source */

        for (i = 0; i < count_src; i++)              /* get past the source tokens */

            (*t) = (*t)->next;

        for (i = 0; i < absolute(count_rule - count_src); i++)     /* foreach i in distance(count_rule,count_src) */

            add_at_current(t," ",0,l);                             /* add an empty token for later filling */


        for (i = 0; i < count_rule; i++)
            (*t) = (*t)->prev;

        IF_DEBUG(printf("debug: %s\n",(*t)->val));
    }

    IF_DEBUG(printf("rule count still %d\n",count_rule));

    for (i = 0; i < count_rule; i++) {
        (*t)->type = define_rules[idx].replace->type;

        char *v = getvarname(define_rules[idx].replace->val,varnames,count_src);

        if (v)
            (*t)->val = strdup(v);
        else
            (*t)->val = strdup(define_rules[idx].replace->val);

        (*t)->line = line;
        *t = (*t)->next;
        define_rules[idx].replace = define_rules[idx].replace->next;
    }

    while (count_src-- > count_rule) {
        IF_DEBUG(printf("> popping %p (at %s, prev %s)",*t,(*t)->val,(*t)->prev->val));
        pop(t);
        IF_DEBUG(printf("> popped %p (at %s, prev %s)\n",*t,(*t)->val,(*t)->prev->val));
    }

    while ((*t)->prev) {
        *t = (*t)->prev;
        IF_DEBUG(printf("went back: %p\n",*t));
    }
}
