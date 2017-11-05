#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "extern.h"

#ifdef UNCOMMENT_MAIN

int flags[127];

int main(int argc, char **argv)
{
    uncomment(argv[1] ? argv[1] : "stdin","stdout",NULL);
}

#endif

/* crappy, yet working perfectly, and rather efficient */

void uncomment__file_based(const char *infile, const char *outfile)
{
    FILE *in  = strcmp(infile,"stdin") ? fopen(infile,"r") : stdin;
    FILE *out = strcmp(outfile,"stdout") ? fopen(outfile,"w+") : stdout;

    if (!in) {
        fprintf(stderr,"Error opening input file %s: %s\n",infile,strerror(errno));
        return;
    }
    if (!out) {
        fprintf(stderr,"Error opening output file %s: %s\n",outfile,strerror(errno));
        return;
    }

    int i = 0;
    int c = 0, prev = 0;
    int line_comment = 0, mline_comment = 0, string = 0;
    char outbuf[1024] = {0};

    while (1)
    {
        c = getc(in);

        if (c == EOF) {
            break;
        }
        else if (c == '"' && prev != '\\') {
            string = !string;
        }
        else if (string) {
            /* dummy to make sure that a comment in a string is not ignored */
        }
        else if (c == '/' && prev == '/') {
            line_comment = 1;
            outbuf[i--] = 0;
        }
        else if (c == '*' && prev == '/') {
            mline_comment = 1;
        }
        else if (c == '\n' && prev != '\\') {
            line_comment = 0;
        }

        if (!line_comment && !mline_comment)
            outbuf[i++] = (char)c;

        if (c == '\n' && prev == '\\') {
            outbuf[i--] = 0;
            outbuf[i--] = 0;
        }

        else if (c == '/' && prev == '*' && !string) {
            outbuf[i--] = 0;
            mline_comment = 0;
        }

        if (i+1 >= 1024) {
            fwrite(outbuf,1,strlen(outbuf),out);
            i = 0;
            memset(outbuf,0,1024);
        }

        prev = c;
    }

    if (strlen(outbuf))
        fwrite(outbuf,1,strlen(outbuf),out);

    fclose(out);
    fclose(in);
}

void uncomment__token_based(token_t **tokens)
{
    int line_comment = 0, mline_comment = 0;

    while ((*tokens)->next)
    {
         if (!strcmp((*tokens)->val,"/*") && !line_comment)
             mline_comment = 1;
         else if (!strcmp((*tokens)->val,"//") && !mline_comment)
             line_comment = (*tokens)->line;
         else if (!strcmp((*tokens)->val,"\\") && line_comment)
             line_comment++;
         else if (!strcmp((*tokens)->val,"*/"))
             mline_comment = 2;
         else if ((*tokens)->line > line_comment)
             line_comment = 0;

         IF_DEBUG(printf("Current token: %s (popping? %s)\n",(*tokens)->val,line_comment || mline_comment ? "yes" : "no"));

         if (line_comment || mline_comment)
             pop(tokens);
         else
             if ((*tokens)->next)
                 *tokens = (*tokens)->next;

         if (mline_comment == 2)
             mline_comment = 0;
    }

    while ((*tokens)->prev)
        *tokens = (*tokens)->prev;
}

void uncomment(const char *infile, const char *outfile, token_t **tokens)
{
    if (flags['c'])
        return;
    if (tokens)
        uncomment__token_based(tokens);
    else
        uncomment__file_based(infile,outfile);
}
