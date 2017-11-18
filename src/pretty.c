#include <stdio.h>
#include <string.h>
#include "pretty.h"
#include "token.h"
#include "types.h"
#include "extern.h"

int opening_brace(const token_t *src, struct prettyprint **ps)
{
    if (src->prev && !strcmp(src->prev->val,"=")) {    /* e.g. char x[10] = {0}; */

        putchar('{');
        return 0;
    }

    int i = 0;

    putchar('\n');

    while (i++ < (*ps)->spaces)
        printf("    ");

    puts("{");
    (*ps)->spaces++;

    i = 0;
    while (i++ < (*ps)->spaces)
        printf("    ");

    return 1;
}

int closing_brace(const token_t *src, struct prettyprint **ps)
{
    if (src->prev->type == TYPE_SEMICOLON) {    /* basically, anything outside of e.g. char x[10] = {0}; */

        (*ps)->spaces = (*ps)->spaces - 1;

        int i = 0;

        putchar('\n');

        while (i++ < (*ps)->spaces)
           printf("    ");

        puts("}");
        return 1;
    }

    putchar('}');

    return 0;
}

int semicolon(token_t *_src, struct prettyprint **ps)
{
    token_t *src = _src;   /* prevent pointer advancement */

    short int in_for = 0;  /* bool: in-for-loop */


    int line = src->line;
    while (src && line == src->line && !in_for) {
        if (!strcmp(src->val,"for"))
            in_for = 1;
        src = src->prev;
    }

    if (in_for) {
        printf("; ");
        return 0;
    }

    putchar(';');

    if (_src->next->type != TYPE_CBRACE) {
        putchar('\n');

        int i = 0;
        while (i++ < (*ps)->spaces)
            printf("    ");

        return 1;
    }

    return 0;
}

int operator(token_t *src, struct prettyprint **ps)
{
    if (!strcmp(src->val,".") || !strcmp(src->val,"->") || !strcmp(src->val,"++") || !strcmp(src->val,"--")) {
        printf("%s",src->val);
        return 0;
    } else if (src->prev->type == TYPE_OPERATOR || src->prev->type == TYPE_OPAREN || src->prev->type == TYPE_COMMA || src->prev->type == TYPE_RESERVED) {
        printf("%s",src->val);
        return 0;
    }

    printf(" %s ",src->val);
    return 0;
}

int keyword(token_t *src, struct prettyprint **ps)
{
    if (!src->prev) {
        printf("%s",src->val);
        return 0;
    }

    char *s = src->prev->val;

    if (isalnum(*s))
        putchar(' ');

    printf("%s",src->val);
    return 0;
}

int control_flow(token_t *src, struct prettyprint **ps)
{
    printf("%s",src->val);

    if (strcmp(src->val,"case") || strcmp(src->val,"default") || strcmp(src->val,"break") || strcmp(src->val,"continue"))
        putchar(' ');

    return 0;
}

void prettyprint_line(int line, token_t *_src, struct prettyprint *ps)
{
    token_t *src = _src;

    while (src && src->line != line)   /* advance pointer location to the line to be printed */

        src = src->next;

    if (src == NULL)                   /* if the source doesn't have the line specified */

        return;                        /* then there's nothing to print */


    while (src && src->line == line)   /* for each specified line in the source */

    {
        int printed_newline = 0;

        if (flags['u']) {
            printf("%s ",src->val);
            src = src->next;
            continue;
        }

        if (src->type == TYPE_OBRACE)
            printed_newline = opening_brace(src,&ps);
        else if (src->type == TYPE_CBRACE)
            printed_newline = closing_brace(src,&ps);
        else if (src->type == TYPE_COMMA)
            printf(", ");
        else if (src->type == TYPE_SEMICOLON)
            printed_newline = semicolon(src,&ps);
        else if (src->type == TYPE_OPERATOR)
            printed_newline = operator(src,&ps);
        else if (src->type == TYPE_OBRACKET)
            putchar('[');
        else if (src->type == TYPE_CBRACKET)
            putchar(']');
        else if (src->type == TYPE_OPAREN)
            putchar('(');
        else if (src->type == TYPE_CPAREN)
            putchar(')');
        else if (src->type == TYPE_LITERAL)
            printf("%s",src->val);
        else if (src->type == TYPE_KEYWORD || src->type == TYPE_RESERVED)
            printed_newline = keyword(src,&ps);
        else if (src->type == TYPE_CONTROL_FLOW)
            printed_newline = control_flow(src,&ps);

        if (printed_newline)
            printed_newline = 0;  /* avoid a -Wall warning since this variable is useless for now */


        src = src->next;
    }
}

#ifdef PRETTY_EXEC

int flags[127];   /* avoid linking errors */


int main(int argc, char **argv)
{
    token_t *tokens, *tmp;
    struct prettyprint ps = {0};
    int line, maxline;
    FILE *in = argv[1] ? fopen(argv[1],"r") : stdin;

    setup_rules();
    tokens = tokenize(in);
    fclose(in);

    uncomment(NULL,NULL,&tokens);

    for (tmp = tokens; tmp->next; tmp = tmp->next)
        ;
    maxline = tmp->line;

    for (line = 1; line <= maxline; line++)
        prettyprint_line(line,tokens,&ps);

    return 0;
}

#endif
