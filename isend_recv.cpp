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


    int cnt=0;
    clock_t start = clock();
    while (cnt < 5) {
        right = (myid + 1) % numprocs;
        left = myid - 1;
        if (left < 0)
            left = numprocs - 1;
        MPI_Irecv(buffer, bsize, MPI_INT, left, 123, MPI_COMM_WORLD, &request);
        MPI_Isend(buffer2, bsize, MPI_INT, right, 123, MPI_COMM_WORLD, &request2);
        MPI_Wait(&request, &status);
        MPI_Wait(&request2, &status);
        cnt++;
        usleep((1000/freq - ((clock()-start)/ (double)(CLOCKS_PER_SEC / 1000)))*1000);
    }
    clock_t end = clock();
    if (myid==0) {
         cout<<"MPI_Isend_Irecv "<<bsize<<" "<<freq<<" "<<((end-start)/( (double)(CLOCKS_PER_SEC / 1000)))/1000<<endl;
    }
    MPI_Finalize();
    return 0;
}