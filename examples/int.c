/* random replace-reserved example */

#rule reserved int_
#rule reserved 32_t
#replace int_ int32_t
#def `int 32_t x` int_ x

int main(void)
{
    int x = INT_MAX;
    int_ y = (int_) x;
    int 32_t z = INT_MIN;
    int_ arr[10] = {0};

    printf("%d\n",y);

    int i;
    for (i = 0; i < 10; i++)
        printf("%d ",arr[i]);

    putchar('\n');
}
