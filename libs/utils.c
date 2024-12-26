#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int getRandIntBetween(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    return min + rand() % (max - min + 1);
}

float minMax(float value, float min, float max) {
    printf("%f %f %f\n", value, min, max);
    if (value < min) {
        printf("menor");
        return min;
    }

    if (value > max) {
        printf("maior");
        return max;
    }

    printf("certo");
    return value;
}
