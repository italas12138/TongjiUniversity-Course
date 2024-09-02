#include <stdio.h>

int drop_egg(int n, int k) {
    int lo = 1, hi = n, mid;
    int drop_count = 0, egg_count = 0;

    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        drop_count++;
        if (mid < k) {
            lo = mid + 1;
            egg_count++;
        } else if (mid > k) {
            hi = mid - 1;
            egg_count++;
        } else {
            egg_count++;
            printf("Egg didn't break at floor %d\n", mid);
            printf("Total drops: %d\n", drop_count);
            printf("Total eggs used: %d\n", egg_count);
            return 0;
        }
    }

    printf("Egg broke at floor %d\n", hi);
    printf("Total drops: %d\n", drop_count);
    printf("Total eggs used: %d\n", egg_count);
    return 1;
}

int main() {
    int n = 100, k = 50;
    drop_egg(n, k);
    return 0;
}