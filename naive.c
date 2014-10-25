#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool *S;

int u;
int n;

int main() {
    scanf("%d", &u);
    S = (bool *)malloc((u + 1) * sizeof(bool));
    
    int t;
    for (t = 0; t <= u; ++ t)
        S[t] = false;

    scanf("%d", &n);
    while (n --) {
        int k, x;
        scanf("%d", &k);
        if (k == 0) {
            scanf("%d", &x); 
            S[x] = true;
        }
        else if (k == 1) {
            scanf("%d", &x); 
            S[x] = false;
        }
        else if (k == 5) {
            int i;
            for (i = u; i >= 0; -- i)
                if (S[i]) break;
            printf("%d\n", i);
        }
        else if (k == 6) {
            int i;
            for (i = 0; i <= u; ++ i)
                if (S[i]) break;
            if (i == u + 1)
                i = -1;
            printf("%d\n", i);
        }
        else if (k == 2) {
            scanf("%d", &x); 
            printf("%d\n", (S[x] ? 1 : 0));
        }
        else if (k == 3) {
            scanf("%d", &x); 
            int i;
            for (i = x - 1; i >= 0; -- i)
                if (S[i]) break;
            printf("%d\n", i);
        }
        else {
            scanf("%d", &x); 
            int i;
            for (i = x + 1; i <= u; ++ i)
                if (S[i]) break;
            if (i == u + 1)
                i = -1;
            printf("%d\n", i);
        }
    }
    free(S);
    return 0;
}
