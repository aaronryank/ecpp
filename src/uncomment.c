#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "extern.h"

// this is probably crappy but I was tired when I wrote this and it works perfectly

#ifdef UNCOMMENT_MAIN

int main(int argc, char **argv)
{
    uncomment(argv[1] ? argv[1] : "stdin","stdout");
}

#endif

void uncomment(const char *infile, const char *outfile)
{
    FILE *in  = strcmp(infile,"stdin") ? fopen(infile,"r") : stdin;
    FILE *out = strcmp(outfile,"stdout") ? fopen(outfile,"w+") : stdout;

    int loop = 1, i = 0;
    int c = 0, prev = 0;
    int line_comment = 0, mline_comment = 0, string = 0;
    char outbuf[1024] = {0};

    while (loop)
    {
        c = getc(in);
        if (c == EOF)
            break;
        else if (c == '"' && prev != '\\') {
            string = !string;
        }
        else if (string) {
            // dummy to make sure that a comment in a string is not ignored
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
            //fprintf(out,"%s",outbuf);
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
