#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Hey! Run it like this: %s <some number>\n", argv[0]);
        return -1;
    }

    int n;
    n = atoi(argv[1]);

    if (n < 0)
    {
        printf("Oops! Negative numbers don\'t have factorials.\n");
        return -1;
    }

    unsigned long long fact = 1;
    int i;
    for (i = 1; i <= n; i++)
    {
        fact = fact * i;
    }

    printf("Wow! The factorial of %d is %llu\n", n, fact);

    return 0;
}
