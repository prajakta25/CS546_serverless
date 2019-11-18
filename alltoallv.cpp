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
/*
This program tests MPI_Alltoallv by having processor i send different
amounts of data to each processor.
The first test sends i items to processor i from all processors.
*/
int main( int argc, char **argv )
{
    MPI_Comm comm;
     int freq,bsize;
     for (int i=1;i<argc;i++) {
        if (i==1) {
            bsize=stoi(argv[i]) *1024;
        }
        if (i==2) {
            freq=stoi(argv[i]);
        }
    }
    int *sbuf, *rbuf;
    int rank, size;
    int *sendcounts, *recvcounts, *rdispls, *sdispls;
    int i, j, *p, err;

    MPI_Init( &argc, &argv );
    err = 0;
    comm = MPI_COMM_WORLD;
    /* Create the buffer */
    MPI_Comm_size( comm, &size );
    MPI_Comm_rank( comm, &rank );
    sbuf = (int *)malloc( bsize * sizeof(int) );
    rbuf = (int *)malloc( bsize * sizeof(int) );
    if (!sbuf || !rbuf) {
        fprintf( stderr, "Could not allocated buffers!\n" );
        MPI_Abort( comm, 1 );
    }
    /* Load up the buffers */
    for (i=0; i<bsize; i++) {
        sbuf[i] = i + 100*rank;
        rbuf[i] = -i;
    }
    /* Create and load the arguments to alltoallv */
    sendcounts = (int *)malloc( size * sizeof(int) );
    recvcounts = (int *)malloc( size * sizeof(int) );
    rdispls = (int *)malloc( size * sizeof(int) );
    sdispls = (int *)malloc( size * sizeof(int) );
    if (!sendcounts || !recvcounts || !rdispls || !sdispls) {
        fprintf( stderr, "Could not allocate arg items!\n" );fflush(stderr);
        MPI_Abort( comm, 1 );
    }
    for (i=0; i<size; i++) {
        sendcounts[i] = i;
        recvcounts[i] = rank;
        rdispls[i] = i * rank;
        sdispls[i] = (i * (i+1))/2;
    }
    double starttime = MPI_Wtime();
    MPI_Alltoallv( sbuf, sendcounts, sdispls, MPI_INT,
                       rbuf, recvcounts, rdispls, MPI_INT, comm );
    double endtime = MPI_Wtime();
    /* Check rbuf */
    for (i=0; i<bsize; i++) {
        p = rbuf + rdispls[i];
        for (j=0; j<rank; j++) {
            if (p[j] != i * 100 + (rank*(rank+1))/2 + j) {
                /*fprintf( stderr, "[%d] got %d expected %d for %dth\n",
                                    rank, p[j],(i*(i+1))/2 + j, j );*/
                fflush(stderr);
                err++;
            }
        }
    }
    free( sdispls );
    free( rdispls );
    free( recvcounts );
    free( sendcounts );
    free( rbuf );
    free( sbuf );
    if (rank ==0) {
         cout<<"MPI_Alltoallv "<<bsize<<" "<<freq<<" "<<endtime-starttime<<endl;
    }
    MPI_Finalize();
    return 0;
}