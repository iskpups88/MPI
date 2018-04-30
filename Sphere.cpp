#include <stdio.h>
#include <mpi.h>
#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//void srand();
double drand48();

#define iterations 2500000

double getVolumes(int n)
{
    double r = 0;
    double globalres, total0, total1 = 0;
    {

        for (long i = 0; i < iterations; i++)
        {
            total0 = 0.0;
            for (int j = 0; j < n; ++j)
            {
                r = (long double)drand48();
                total0 += r * r;
            }
            if (total0 < 1)
                ++total1;
        }
    }
    MPI_Reduce(&total1, &globalres, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return globalres;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int Procs;
    int my_rank;
    double globalres;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Procs);
    srand48((long)my_rank);
    for (int i = 2; i < 20; i++)
    {
        globalres = getVolumes(i);
        if (my_rank==0)
        printf("%d-dimensional sphere value = %f\n", i, pow(2.0, i) * (globalres / (iterations*Procs)));
    }
}
