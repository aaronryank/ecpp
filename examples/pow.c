/* python-style to-the-power-of operator */

#include <stdio.h>
#include <math.h>

#def x**y pow(x,y)

int main(int argc, char **argv)
{
    int x = atoi(argv[1]), y = atoi(argv[2]);

    printf("x ** y: %d\n",(int) x ** y);
}
