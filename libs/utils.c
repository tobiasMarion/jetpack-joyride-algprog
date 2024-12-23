#include <time.h>
#include <stdlib.h>

int getRandIntBetween(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    return min + rand() % (max - min + 1);
}
