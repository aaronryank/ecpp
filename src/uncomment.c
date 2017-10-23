#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

// this is probably crappy but I was tired when I wrote this and it works perfectly

int main(int argc, char **argv)
{
    FILE *in = (argc > 1) ? fopen(argv[1],"r") : stdin;

    if (!in) {
        fprintf(stderr,"Error %d: Could not open file %s: %s\n",errno,argv[1],strerror(errno));
        return 1;
    }

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
            printf("%s",outbuf);
            i = 0;
            memset(outbuf,0,1024);
        }

        prev = c;
    }

    printf("%s",outbuf);
}