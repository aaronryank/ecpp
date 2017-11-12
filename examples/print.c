#include <stdio.h>

#replace print <<<
#def `<<< y` puts(y)

int main(void)
{
    print "Hello, World!";
}
