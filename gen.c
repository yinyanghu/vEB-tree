#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define u       1024*1024*16*4
#define n       100000
#define first   n/10

int main() {
    srand(time(NULL));
    printf("%d\n", u);
    printf("%d\n", n);
    int i;
    for (i = 0; i < first; ++ i)
        printf("0 %d\n", rand() % (u + 1));
    for (i = first; i < n; ++ i) {
        int k = rand() % 7;
        if (k <= 4)
            printf("%d %d\n", k, rand() % (u + 1));
        else
            printf("%d\n", k);
    }
    return 0;
}
