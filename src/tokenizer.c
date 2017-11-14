#include <stdio.h>
#include <string.h>
#include "token.h"
#include "types.h"
#include "string.h"
#include "extern.h"

#define whitespace(c) (c == ' ' || c == '\n' || c == '\t' || c == '\r')

token_t *tokenize(FILE *in)
{
    token_t *tokens = NULL;
    short int loop = 1;
    int prev = 0, line = 1;

    while (loop)
    {
        int in_token = 1, i = 0;
        char str[200] = {0};

        while (in_token)
        {
            int c = getc(in);

            /* cpp or ecpp lines are ignored */
            if (c == '#' && (prev == '\n' || prev == 0)) {
                while ((c = getc(in)) != '\n')
                    continue;
                c = 0;
                in_token = 0;
            }
            else if (c == EOF || (whitespace(c) && !str_contains(str,"\'\""))) {
                in_token = 0;
                loop = (c != EOF);
                if (strlen(str))
                    push(&tokens,str,gettype(str),line);
            }
            else {
                if (compatibletypes(str,c)) {   /* if adding c to str creates a valid token... */
                    str[i++] = c;               /* add it                                      */
                }
                else {
                    in_token = 0;
                    push(&tokens,str,gettype(str),line);
                    ungetc(c,in);
                }
            }

            if (c == '\n')
                line++;

            prev = c;
        }
    }

    rewind_token(&tokens);
    rewind(in);
    return tokens;
}

int gettype(const char *token)
{
    if (str_eq(token,(const char **)RESERVED_KEYWORDS,reserved_keyword_count)) {
        return TYPE_RESERVED;
    }
    else if (str_eq(token,(const char **)CONTROL_FLOW_KEYWORDS,control_flow_keyword_count)) {
        return TYPE_CONTROL_FLOW;
    }
    else if (iskeyword(token)) {
        return TYPE_KEYWORD;
    }
    else if (!strcmp(token,"(")) {
        return TYPE_OPAREN;
    }
    else if (!strcmp(token,")")) {
        return TYPE_CPAREN;
    }
    else if (!strcmp(token,"[")) {
        return TYPE_OBRACKET;
    }
    else if (!strcmp(token,"]")) {
        return TYPE_CBRACKET;
    }
    else if (!strcmp(token,"{")) {
        return TYPE_OBRACE;
    }
    else if (!strcmp(token,"}")) {
        return TYPE_CBRACE;
    }
    else if (!strcmp(token,",")) {
        return TYPE_COMMA;
    }
    else if (!strcmp(token,";")) {
        return TYPE_SEMICOLON;
    }
    else if (isliteral(token)) {
        return TYPE_LITERAL;
    }
    else if (isoperator(token)) {
        return TYPE_OPERATOR;
    }
    return TYPE_UNDEF;
}

short int compatibletypes(const char *s, int c)
{
    int type = gettype(s);

    if (!strlen(s)) {    /* if token is empty,                              */
        return 1;        /* of course adding c to it creates a valid token  */
    }
    else if (type == TYPE_OPAREN ||    \
             type == TYPE_CPAREN ||    \
             type == TYPE_OBRACKET ||  \
             type == TYPE_CBRACKET ||  \
             type == TYPE_OBRACE ||    \
             type == TYPE_CBRACE ||    \
             type == TYPE_COMMA ||     \
             type == TYPE_SEMICOLON) {     /* single-character types */

        return 0;
    }
    else if (str_contains(s,"\'\"")) {  /* if token is a string or character */

        if (type == TYPE_LITERAL)       /* if string/character is already finished */

            return 0;                   /* then additional characters will not be part of the same token */

        return 1;                       /* otherwise, they will */

    }
    else if (isnumliteral(s)) {        /* if it's a number literal */

        if (strchr("0123456789.L",c))  /* if the character is one of the characters making up a valid number literal */

            return 1;                  /* then it can be added */

        return 0;                      /* otherwise it will be part of the next token */

    }
    else if (type == TYPE_RESERVED || type == TYPE_CONTROL_FLOW || type == TYPE_KEYWORD) {  /* if token is a valid keyword */

        if (strchr(LOWERCASE_ALPHA UPPERCASE_ALPHA "0123456789_",c))                        /* if character is alphanumerical or an underscore */

            return 1;                                                                       /* it can be appended */

        return 0;                                                                           /* can't otherwise */

    }
    else if (type == TYPE_OPERATOR) {          /* if token is a valid operator */

        if (strchr("`~!@#$%^&*-+=|<>/?:.",c))  /* and the character is too */

            return 1;                          /* then together they are also valid operators */

        return 0;                              /* otherwise not */

    }
    return 0;    /* if something is messed up then it's assumed to be invalid */

}
