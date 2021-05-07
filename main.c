#include "set.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void single_layer_convolution (int M, int N, float **input,
int K, float **kernel,
float **output)

{
    int i,j,ii,jj;
    double temp;
    for (i=0; i<=M-K; i++)
        for (j=0; j<=N-K; j++) {
            temp = 0.0;

            for (ii=0; ii<K; ii++)
                for (jj=0; jj<K; jj++)
                    temp += input[i+ii][j+jj]*kernel[ii][jj];
                output[i][j] = temp;
    }
    return;
}


int compareoutput(float **output1, float **output2, int M, int N, int K)

{
    for (int i = 0; i < M - K + 1; i++){
        for (int j = 0; j < N - K + 1; j++){
            if (output1[i][j] != output2[i][j]) return 0;  
        }
    }
    return 1;
}


int main(int argc, char *argv[])
{
    int M, N, K, root = 0, nprocs, rank; 

    float **input=NULL, **output=NULL, **kernel=NULL;

    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

    if (rank == root) {
        M = atoi(argv[1]);
        N = atoi(argv[2]);
        K = atoi(argv[3]);

        // allocate 2D array ’input’ with M rows and N columns
        input = malloc(M * sizeof(float*));
        input[0] = malloc(M * N * sizeof(float));

        for(int i = 0; i< M; i++)
            input[i] = &(input[0][i * N]);

        // allocate the convolutional kernel with K rows and K columns
        kernel = malloc(K * sizeof(float*));
        kernel[0] = malloc(K * K * sizeof(float));

        for(int i = 0; i < K; i++){
            kernel[i] = &(kernel[0][i * K]);
        }

        // allocate 2D array ’output’ with M-K+1 rows and N-K+1 columns
        output = malloc((M-K+1) * sizeof(float*));
        output[0] = malloc((M-K+1) * (N-K+1) * sizeof(float));

        for(int i = 0; i< M - K + 1; i++){
            output[i] = &(output[0][i * (N - K + 1)]);
        }

        //fill 2D array ’input’ with some values
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                input[i][j] = 0.01*i + 0.01*j;
                }
            }

        // fill kernel with some values
        for (size_t i = 0; i < K; i++) {
            for (size_t j = 0; j < K; j++) {
                kernel[i][j] = i + j;
                }
            }
    }

    // root process broadcasts values of M, N, K to all the other processes
    MPI_Bcast(&M, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&N, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&K, 1, MPI_INT, root, MPI_COMM_WORLD);

    if (rank != root) {
    // allocate the convolutional kernel with K rows and K columns
    kernel = malloc(K * sizeof(float*));
    kernel[0] = malloc(K * K * sizeof(float));

    for(int i = 0; i < K; i++)
        kernel[i] = &(kernel[0][i * K]);
    }

    // root process broadcasts the content of kernel to all the other processes
    MPI_Bcast(&(kernel[0][0]), K * K, MPI_FLOAT, root, MPI_COMM_WORLD);

    // parallel computation of a single-layer convolution
    double clock = MPI_Wtime();
    MPI_single_layer_convolution (M, N, input, K, kernel, output);
    double paralleltime = MPI_Wtime() - clock;


    if (rank==root) {
        float **expected = malloc((M-K+1) * sizeof(float*));
        expected[0] = malloc((M-K+1) * (N-K+1) * sizeof(float));

        for(int i = 0; i< M - K + 1; i++){
            expected[i] = &(expected[0][i * (N - K + 1)]);
        }
        clock = MPI_Wtime();
        single_layer_convolution(M, N, input, K, kernel, expected);
        double serialtime = MPI_Wtime() - clock;

        //check the potential speedup of running parallel
        printf("Running time in serial is %lf \n", serialtime);
        printf("Running time in parallel is %lf \n", paralleltime);
        
        // compare the content of array ’output’ with that is produced by the sequential function single_layer_convolution
        if (compareoutput(output, expected, M, N, K) == 1){
            printf("Serial output matches parallel output \n");
        }
        free(expected[0]);
        free(expected);
    }

    MPI_Finalize();

    free(kernel[0]);
    free(kernel);
    if (rank == root){
        free(output[0]);
        free(output);
        free(input[0]);
        free(input);
    }
    return 0; 
}
