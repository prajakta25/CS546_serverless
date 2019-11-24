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
    int freq,bsize;
    int i, rank, reps, n;
     for (int i=1;i<argc;i++) {
        if (i==1) {
            bsize=stoi(argv[i])*1024;
        }
        if (i==2) {
            freq=stoi(argv[i]);
        }
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int cnt=0;
    clock_t start = clock();
    while (cnt < 5) {
        int *buf = (int *) malloc(sizeof(int)*bsize);
        int *recvBuf = (int *) malloc(sizeof(int)*bsize);
        if (rank == 0) {
            memset(buf,4,bsize * sizeof(int));
        }

        MPI_Scatter(buf, bsize, MPI_INT, recvBuf,bsize, MPI_INT, 0, MPI_COMM_WORLD);
        cnt++;
        usleep((1000/freq - ((clock()-start)/ (double)(CLOCKS_PER_SEC / 1000)))*1000);
    }
    clock_t end = clock();
    if (rank ==0) {
        cout<<"MPI_Scatter "<<bsize<<" "<<freq<<" "<<((end-start)/( (double)(CLOCKS_PER_SEC / 1000)))/1000<<endl;
    } 
    MPI_Finalize();
    return 0;
}