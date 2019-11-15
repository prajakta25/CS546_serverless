#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <iostream>
using namespace std;
/*
 * Sort array using blocking send/recv between 2 ranks.
 *
 * The master process prepares the data and sends the latter half
 * of the array to the other rank. Each rank sorts it half. The
 * master then merges the sorted halves together. The two ranks
 * communicate using blocking send/recv.
 */

#define NUM_ELEMENTS 50

static int compare_int(const void *a, const void *b)
{
    return (*(int *) a - *(int *) b);
}

/* Merge sorted arrays a[] and b[] into a[].
 * Length of a[] must be sum of lengths of a[] and b[] */
static void merge(int *a, int numel_a, int *b, int numel_b)
{
    int *sorted = (int *) malloc((numel_a + numel_b) * sizeof *a);
    int i, a_i = 0, b_i = 0;
    
    /* merge a[] and b[] into sorted[] */
    for (i = 0; i < (numel_a + numel_b); i++) {
        if (a_i < numel_a && b_i < numel_b) {
            if (a[a_i] < b[b_i]) {
                sorted[i] = a[a_i];
                a_i++;
            } else {
                sorted[i] = b[b_i];
                b_i++;
            }
        } else {
            if (a_i < numel_a) {
                sorted[i] = a[a_i];
                a_i++;
            } else {
                sorted[i] = b[b_i];
                b_i++;
            }
        }
    }
    /* copy sorted[] into a[] */
    memcpy(a, sorted, (numel_a + numel_b) * sizeof *sorted);
    free(sorted);
}

int main(int argc, char **argv)
{
    int rank, size;
    int freq,bsize;
     for (int i=1;i<argc;i++) {
        if (i==1) {
            bsize=stoi(argv[i])*1024;
        }
        if (i==2) {
            freq=stoi(argv[i]);
        }
    }
    int *data = (int *) malloc(sizeof(int)*bsize);
    double startwtime = 0.0, endwtime;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int partitionLen[size];
    //find the data size for each processor
    int partition = floor((double) bsize / size);

    //store the data size of each processor in an array
    for (int i=0;i<size; i++) {
        //if data partition size is uneven then the last processor will have extra elements compared to other processors
        if (i != size-1) {
            partitionLen[i] = partition;
        } else {
            //if the data is unevenly distributed, find the no of elements that are unallocated and add it to the elements of last processor
            int remainingEle = bsize - (partition * size);
            partitionLen[i] = partition + remainingEle;
        }
    }

    srand(0);

    if (rank == 0)
        startwtime = MPI_Wtime();

    if (rank == 0) {
        /* prepare data and display it */
        int i;
        //printf("Unsorted:\t");
        for (i = 0; i < bsize; i++) {
            data[i] = rand() % bsize;
        }

        //distribute the data to other N processors
        for (int i=1 ; i<size ; i++) {
            //s contains the data size for each processor
            int s = partitionLen[i];
            /* if elese is required if the data distribution is uneven among the processors, as the last processor will contain more
            elements*/
            if (i!=size-1) {
                 MPI_Send(&data[s*i], s, MPI_INT, i, 0, MPI_COMM_WORLD);
            } else {
                //k is needed to calculate the address location of the partition in the data
                //if the data is ditributed unevenly then the partition size will be different for the last processor
                int k=partitionLen[0];
                MPI_Send(&data[k*i], s, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
            
        }

        //sort the first part of the partitioned data
        qsort(data, partitionLen[0], sizeof(int), compare_int);

        //receive sorted data from other processors
        for(int i=1;i<size; i++) {
            int s = partitionLen[i];
             if (i!=size-1) {
                 MPI_Recv(&data[s*i], s, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                int k=partitionLen[0];
                MPI_Recv(&data[k*i], s, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            
        }
       
       //merge the data from all the processors
        for (int i=1;i<size;i++) {
            int s=partitionLen[i];
            if (i!=size-1) {
            merge(data,s*i,&data[s*i],s);
            } else {
                int k=partitionLen[0];
                merge(data,k*i,&data[k*i],s);
            }
        }   

    } else {

        //partitionLen array contains the partition size of each  
        int s = partitionLen[rank];
        /* each processor receives their respective part of the data */
        MPI_Recv(data, s, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /* sort the received data */
        qsort(data, s, sizeof(int), compare_int);

        /* send back the sorted data */
        MPI_Send(data, s, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank ==0) {
        endwtime = MPI_Wtime();
        //printf("wall clock time = %f\n", endwtime - startwtime);
        cout<<"{\"Send_Receive\" :"<<(endwtime-startwtime)<<"}"<<endl;
    }
    
    MPI_Finalize();
    return 0;
}

