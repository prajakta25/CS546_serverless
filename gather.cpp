#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
using namespace std;

int main( int argc, char **argv )
{
    int freq,bsize;
    int i, rank, reps, n, size;
     for (int i=1;i<argc;i++) {
        if (i==1) {
            bsize=stoi(argv[i])*1024;
        }
        if (i==2) {
            freq=stoi(argv[i]);
        }
    }
    int *isend, *irecv;
    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
     int cnt=0;
    clock_t start = clock();
    while (cnt < 5) {
        isend = (int *) malloc(sizeof(int)*bsize);
        irecv = (int *) malloc(sizeof(int)*bsize);
         if (rank == 0) {
        memset(isend,4,bsize * sizeof(int));
        }
        MPI_Gather(&isend, 1, MPI_INT, &irecv, 1, MPI_INT, 0, MPI_COMM_WORLD);
        cnt++;
        usleep((1000/freq - ((clock()-start)/ (double)(CLOCKS_PER_SEC / 1000)))*1000);
    }

    clock_t end = clock();
    if (rank ==0) {
        cout<<"MPI_Gather "<<bsize/1024<<" "<<freq<<" "<<((end-start)/( (double)(CLOCKS_PER_SEC / 1000)))/1000<<endl;
    } 

    MPI_Finalize();
    return 0;
}