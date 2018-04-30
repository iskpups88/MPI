#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>

#define DIMERSION_Val 20
#define ITERATION 10000000

double Points[DIMERSION_Val];

void calculateVolume(double *totalPoints) {
    double sum;
    double Point[DIMERSION_Val];

#pragma omp parallel for private(sum) shared(Points) reduction(+:Point) num_threads(4)
    for (int i = 0; i < ITERATION; i++) {
        for (int k = 0; k < DIMERSION_Val; k++) {
            Point[k] = 0;
        }
        sum = 0;
        for (int j = 1; j < DIMERSION_Val; j++) {
            double r = 2 * (((double) rand() / (RAND_MAX)) - 0.5);
            sum = sum + r * r;
            if (sum < 1) {
                Point[j]++;
            } else {
                break;
            }
        }
        #pragma omp critical
        for (int k = 0; k < DIMERSION_Val; k++) {
            totalPoints[k] += Point[k];
        }
    }
}		


int main(int argc, char *argv[]) {
    srand(time(NULL));
    double start = omp_get_wtime();
    for (int i = 0; i < DIMERSION_Val; i++) {
        Points[i] = 0;
    }
    calculateVolume(Points);
    double finish = omp_get_wtime();

    for (int i = 1; i < DIMERSION_Val; i++) {
        double volume = (Points[i] / ITERATION) * pow(2.0, i);
        printf("Volume of %d-Dimension sphere = %f\n", i, volume);
    }
    printf("Time %f\n", finish - start);
}



