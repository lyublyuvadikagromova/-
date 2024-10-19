#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_points>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0 || n > 255) {
        fprintf(stderr, "Number of points must be positive and <= 255\n");
        return 1;
    }

    int inside_circle = 0;
    double x, y;

    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        x = (double)rand() / RAND_MAX * 2 - 1;
        y = (double)rand() / RAND_MAX * 2 - 1;

        if (x * x + y * y <= 1) {
            inside_circle++;
        }
    }

    return inside_circle;
}
