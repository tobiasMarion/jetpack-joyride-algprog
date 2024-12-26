#include <stdlib.h>
#include <time.h>

int getRandIntBetween(int min, int max) {
    srand(time(NULL));

    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    return min + rand() % (max - min + 1);
}

float minMax(float value, float min, float max) {
    if (value < min) {
        return min;
    }

    if (value > max) {
        return max;
    }

    return value;
}
