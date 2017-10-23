/* foreach syntax */

#include <stdlib.h>
#include <stdio.h>

%rule control_flow foreach
%rule control_flow in
%define `foreach(x in y..z)` int x; for (x = y; x <= z; x++)

int main(int argc, char **argv)
{
    int x = atoi(argv[1]), y = atoi(argv[2]);

    foreach(i in x..y)
    {
        printf("%d ",i);
    }
}
