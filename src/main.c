#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "string.h"
#include "token.h"
#include "extern.h"
#include "pretty.h"

int flags[127];

int main(int argc, char **argv)
{
    char *fname = parse_args(argc,argv);   /* get filename and flags */


    FILE *in = fname ? fopen(fname,"r") : stdin;

    if (!in) {
        fprintf(stderr,"Error %d: Could not open file %s: %s\n",errno,argv[1],strerror(errno));
        return 1;
    }

    setup_rules();   /* set up reserved keywords and control flow keywords  */


    read_rules(in);  /* I'm sorry, world */


    IF_DEBUG(puts("--- begin tokenizing ---"));

    token_t *tokens = tokenize(in);

    IF_DEBUG(puts("--- done tokenizing ---"));
    IF_DEBUG(print_list(tokens));

    IF_DEBUG(puts("--- uncommenting ---"));

    uncomment(NULL,NULL,&tokens);

    int line = 1;
    struct prettyprint ps = {0};
    char *s = malloc(1024 * sizeof(char));
    memset(s,0,1024);
    while (fgets(s,1023,in))
    {
        IF_DEBUG(printf("read loop %d debug: %p %s",line,s,s));

        if (s[0] == '#') {
            printf("%s",s);
        }
        else if (s[0] == '%' && !(strstr(s,"rule") == s+1)) {    /* prevent duplication of rules */

            preprocess(s);
        } else {
            tokens = preprocess_line(line,tokens);               /* performs define's and replace's on the current line */

            prettyprint_line(line++,tokens,&ps);
        }

        IF_DEBUG(printf("check %d\n",line-1));
        IF_DEBUG(print_list(tokens));
    }
    putchar('\n'); /* :P */

    free(s);

    IF_DEBUG(puts("--- done preprocessing ---"));
    IF_DEBUG(print_list(tokens));

    free(tokens);
    fclose(in);

    return 0;
}

char *parse_args(int argc, char **argv)
{
    char *fname = NULL;

    int i, j;
    for (i = 1; i < argc; i++) {
        if (strchr(argv[i],'-') == argv[i]) {
            for (j = 1; argv[i][j]; j++)
                flags[(int)argv[i][j]] = 1;
        } else {
            fname = argv[i];
        }
    }

    return fname;
}

void read_rules(FILE *in)
{
    char *s = malloc(1024 * sizeof(char));
    memset(s,0,1024);

    while (fgets(s,1023,in))
        if (*s == '%' && strstr(s,"rule") == s+1)    /* hardcode for rules, they need to be set up before tokenization */

            preprocess(s);

    rewind(in);
    free(s);
}
