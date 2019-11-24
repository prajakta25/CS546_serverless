#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#define ROOT 0
#define NUM_REPS 5
#define NUM_SIZES 1
using namespace std;
int main( int argc, char **argv)
{
    int *buf;
    int freq,bsize;
    int i, rank, reps, n;
    int bVerify = 1;
    int num_errors=0, tot_errors;
    for (int i=1;i<argc;i++) {
        if (i==1) {
            bsize=stoi(argv[i])*1024;
        }
        if (i==2) {
            freq=stoi(argv[i]);
        }
    }
        int sizes[NUM_SIZES] = {bsize};
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (argc > 1)
    {
        if (strcmp(argv[1], "-novalidate") == 0 || strcmp(argv[1], "-noverify") == 0)
            bVerify = 0;
    }
    clock_t start;
    buf = (int *) malloc(sizes[NUM_SIZES-1]*sizeof(int));
    memset(buf, 0, sizes[NUM_SIZES-1]*sizeof(int));
    for (n=0; n<NUM_SIZES; n++)
    {
        if (rank == ROOT)
        {
            fflush(stdout);
        }

        start = clock();
        for (reps=0; reps < NUM_REPS; reps++)
        {
            if (bVerify)
            {
                if (rank == ROOT)
                {
                    for (i=0; i<sizes[n]; i++)
                    {
                        buf[i] = 1000000 * (n * NUM_REPS + reps) + i;
                    }
                }
                else
                {
                    for (i=0; i<sizes[n]; i++)
                    {
                        buf[i] = -1 - (n * NUM_REPS + reps);
                    }
                }
            }
            MPI_Bcast(buf, sizes[n], MPI_INT, ROOT, MPI_COMM_WORLD);
            if (bVerify)
            {
                num_errors = 0;
                for (i=0; i<sizes[n]; i++)
                {
                    if (buf[i] != 1000000 * (n * NUM_REPS + reps) + i)
                    {
                        num_errors++;
                        if (num_errors < 10)
                        {
                            cout<<"Error: Rank="<<rank<<" , n="<<n<<" ,reps="<<reps<<" , i="<<i<<" , buf[i]="<<buf[i]<<" ,expected="<< 1000000 * (n * NUM_REPS + reps) +i<<endl;
                            fflush(stdout);
                        }
                    }
                }
                if (num_errors >= 10)
                {
                    cout<<"Error: Rank="<<rank<<" , num_errors = "<<num_errors<<endl;
                    fflush(stdout);
                }
            }
            usleep((1000/freq - ((clock()-start)/ (double)(CLOCKS_PER_SEC / 1000)))*1000);
        }
    }
    clock_t end = clock();

    MPI_Reduce( &num_errors, &tot_errors, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD );
    if (rank == 0 && tot_errors == 0) 
         cout<<"MPI_Bcast "<<bsize<<" "<<freq<<" "<<((end-start)/( (double)(CLOCKS_PER_SEC / 1000)))/1000<<endl;
    fflush(stdout);
    free(buf);
    MPI_Finalize();
    return 0;
}