#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
    int myid, numprocs, left, right;
    int freq,bsize;
     for (int i=1;i<argc;i++) {
        if (i==1) {
            bsize=stoi(argv[i])*1024;
        }
        if (i==2) {
            freq=stoi(argv[i]);
        }
    }
    int *buffer =(int *) malloc(sizeof(int)*bsize);
    int *buffer2 = (int *) malloc(sizeof(int)*bsize);
    MPI_Request request, request2;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    right = (myid + 1) % numprocs;
    left = myid - 1;
    if (left < 0)
        left = numprocs - 1;
    double starttime=MPI_Wtime();
    MPI_Irecv(buffer, bsize, MPI_INT, left, 123, MPI_COMM_WORLD, &request);
    MPI_Isend(buffer2, bsize, MPI_INT, right, 123, MPI_COMM_WORLD, &request2);
    MPI_Wait(&request, &status);
    MPI_Wait(&request2, &status);
    double endtime=MPI_Wtime();
    if (myid==0) {
         cout<<"MPI_Isend_Irecv "<<bsize<<" "<<freq<<" "<<endtime-starttime<<endl;
    }
    MPI_Finalize();
    return 0;
}