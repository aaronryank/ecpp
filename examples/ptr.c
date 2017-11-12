/* an example of using C++ style pointers (I think) */

#include <stdio.h>

#def int&y int*y

void pass(int& x)
{
    *x = 8;
}

int main(void)
{
    int foo = 3;
    pass(&foo);

    printf("%d\n",foo);
}
