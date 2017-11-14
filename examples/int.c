/* random replace-reserved example */

#rule reserved int_
#replace int_ int32_t

int main(void)
{
    int x = INT_MAX;
    int_ y = (int_) x;

    printf("%d\n",y);
}
