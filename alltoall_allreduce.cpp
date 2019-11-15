#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <iostream>
using namespace std;

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif

int main( int argc, char *argv[] )
{
    int rank, size;
    int freq,chunk;
     for (int i=1;i<argc;i++) {
        if (i==1) {
            chunk=stoi(argv[i])*1024;
        }
        if (i==2) {
            freq=stoi(argv[i]);
        }
    }
    int i;
    int *sb;
    int *rb;
    int status, gstatus;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    sb = (int *)malloc(size*chunk*sizeof(int));
    if ( !sb ) {
        perror( "can't allocate send buffer" );fflush(stderr);
        MPI_Abort(MPI_COMM_WORLD,EXIT_FAILURE);
    }
    rb = (int *)malloc(size*chunk*sizeof(int));
    if ( !rb ) {
        perror( "can't allocate recv buffer");fflush(stderr);
        free(sb);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    for ( i=0 ; i < size*chunk ; ++i ) {
        sb[i] = rank + 1;
        rb[i] = 0;
    }
    double starttime=MPI_Wtime();
    status = MPI_Alltoall(sb, chunk, MPI_INT, rb, chunk, MPI_INT, MPI_COMM_WORLD);
    MPI_Allreduce( &status, &gstatus, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
    double endtime=MPI_Wtime();
    if (rank == 0) {
        cout<<"{\"AlltoAll_Allreduce\" :"<<(endtime-starttime)<<"}"<<endl;
        if (gstatus != 0) {
            printf("all_to_all returned %d\n",gstatus);fflush(stdout);
        }
    }
    free(sb);
    free(rb);
    MPI_Finalize();
    return(EXIT_SUCCESS);
}