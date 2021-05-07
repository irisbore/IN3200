#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 
#include "set.h"

void MPI_single_layer_convolution (int M, int N, float **input,
int K, float **kernel,
float **output)
{
    int rank, nprocs;
    
    int *sendcounts = NULL, *recvcounts = NULL, *Sdispls = NULL, *Gdispls = NULL;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    int root = 0;

    // Calculate displacements and number of rows for each process.
    int rest = (M - K + 1) % nprocs;
    int part = (M - K + 1) / nprocs; // Create chunk for each process with respect to output size
    int myrest = part + (rank < rest); // Make sure all leftover rows are distributed


    if (rank == root){
        // Used when scattering input.
        sendcounts = malloc(nprocs*sizeof *sendcounts);
        Sdispls = malloc(nprocs*sizeof *Sdispls);
            
        // Used when gathering y
        recvcounts = malloc(nprocs * sizeof *recvcounts);
        Gdispls = malloc(nprocs*sizeof *Gdispls);

        //A 1D block-wise decomposition with respect to the rows
        // First remainder processes gets an extra row.
        Sdispls[0] = 0;
        Gdispls[0] = 0;

        for (int r = 0; r < nprocs - 1; r++) {
            sendcounts[r] = (part + K - 1 + (r < rest)) * N;
            recvcounts[r] = (part + (r < rest)) * (N - K + 1);
            Sdispls[r+1] = Sdispls[r] + sendcounts[r] - (K - 1) * N; // Subtract to include ghost layer 
            Gdispls[r+1] = Gdispls[r] + recvcounts[r];
        }
        sendcounts[nprocs-1] = (part + K - 1) * N ;
        recvcounts[nprocs-1] = part * (N - K + 1);
    } 

    // Allocate local buffers (each get their own copy)
    if (rank != root){
        input = malloc((myrest + K - 1) * sizeof(float*));
        input[0] = malloc((myrest + K - 1) * N * sizeof(float));
        for(int i = 0; i < (myrest + K - 1); i++)
            input[i] = &input[0][i * N];
        
        output = malloc(myrest * sizeof(float*));
        output[0] = malloc(myrest * (N - K + 1) * sizeof(float));
        for(int i = 0; i < myrest; i++)
            output[i] = &output[0][i * (N - K + 1)]; 
       
    }
  
    // Scatter input
    MPI_Scatterv(&(input[0][0]),                 // Sendbuff, matters only for root process.
                 sendcounts,
                 Sdispls,
                 MPI_FLOAT,
                 &(input[0][0]),                 // Recieve buff is local input array
                 (myrest + K - 1) * N,           // Add K - 1 to get local input size                                    
                 MPI_FLOAT,
                 root,
                 MPI_COMM_WORLD);
    
    //Perform convolution on each process
    single_layer_convolution(myrest + K - 1, N, input, K, kernel, output);
    
    //Gather local outputs
    MPI_Gatherv(&(output[0][0]) , (myrest) * (N - K + 1), MPI_FLOAT, &(output[0][0]) , recvcounts, Gdispls , MPI_FLOAT ,root , MPI_COMM_WORLD);

    //Free those who are done
    if (rank == root){
            free(sendcounts);
            free(recvcounts);
            free(Sdispls);
            free(Gdispls);
    }
        else{
            free(input[0]);
            free(input);
            free(output[0]);
            free(output);
            
        }
    
   return;
}