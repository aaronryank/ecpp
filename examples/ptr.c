/* an example of using C++ style pointers (I think) */

#include <stdio.h>

#rule reserved type
#def type&y type*y

void pass(type& x)
{
    *x = 8;
}

int main(void)
{
    int foo = 3;
    pass(&foo);

    printf("%d\n",foo);
}
