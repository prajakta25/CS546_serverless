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
    int *buf = (int *) malloc(sizeof(int)*bsize);
    int *recvBuf; 
    if (rank == 0) {
       memset(buf,4,bsize * sizeof(int));
       recvBuf = (int *) malloc(sizeof(int)*bsize);
    }
    double starttime = MPI_Wtime();
    MPI_Gather( buf, bsize, MPI_INT, recvBuf, bsize, MPI_INT, 0, MPI_COMM_WORLD); 
    double endtime = MPI_Wtime();
    if (rank ==0) {
        cout<<"MPI_Gather "<<bsize<<" "<<freq<<" "<<endtime-starttime<<endl;
    } 
    MPI_Finalize();
    return 0;
}