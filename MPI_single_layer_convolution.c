#include <stdio.h>
#include <mpi.h> 
#include <stdlib.h>
#define idx(i,j) (i*N + j)
//usr/lib64/openmpi/bin/mpicc
//usr/lib64/openmpi/bin/mpirun -np number ./a.out

/*  
the input 2D array is of dimension M ×N (M rows, N columns)
and the convolutional kernel is of dimension K ×K. The output 2D array is of dimension (M −K +1)×(N −K +1).

possible strategy is to let each process (except rank 0)
send its subdomain result to process rank 0, which sums up all
the subdomain results

distribution (alternative to broadcast):
MPI_Gather() collects the send buffer contents of all processes and concatenates them in rank order 
into the receive buffer of the root process (like rank 0)
MPI_Scatter() does the reverse, distributing equal-sized chunks of the root’s send buffer.
variants (with a “v” appended to their names) that support arbitrary per-rank chunk sizes

free(output[0]);
    free(output);
    free(input[0]);
    free(input);
    free(kernel[0]);
    free(kernel);
    free(sendcounts);

make sure nprocs is larget than M N K 
*/

void MPI_single_layer_convolution (int M, int N, float **input,
int K, float **kernel,
float **output)
{
    int rank, nprocs;
    
    int *sendcounts = NULL, *gathercounts = NULL, *Sdispls = NULL, *Gdispls = NULL;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    
    printf("rank: %d/%d\n", rank, nprocs);
   
    int root = 0;
    
    // Calculate displacements and number of rows for each process.
    int *n_rows = malloc(nprocs*sizeof *n_rows);
    int rest = M % nprocs;
    int part = M / nprocs;
    int myrest = part + (rank < rest);
    int down = K - 1; 

    if (rank == root){
        // Used when scattering input.
        sendcounts = malloc(nprocs*sizeof *sendcounts);
        Sdispls = malloc(nprocs*sizeof *Sdispls);

        // Used when gathering y
        Gdispls = malloc(nprocs*sizeof *Gdispls);

        Sdispls[0] = 0;
        Gdispls[0] = 0;

        //a 1D block-wise decomposition with respect to the rows
        // Last remainder processes gets an extra row.

        for (int r = 0; r < nprocs-1; r++) {
            n_rows[r] = part + (r < rest ? 1:0);
            sendcounts[r] = n_rows[r] * N;
            Sdispls[r+1] = Sdispls[r] + sendcounts[r];
            Gdispls[r+1] = Gdispls[r] + n_rows[r];
        }
        n_rows[nprocs - 1] = part + (nprocs - 1 < rest ? 1:0);
        sendcounts[nprocs-1] = n_rows[nprocs-1] * N;
        printf("done with decomp \n");

    } 

    //int rows = M /nprocs;
    //n_rows[nprocs-1] = rows + ((nprocs-1) >= (nprocs - remainder) ? 1:0);

    /*
    int _M = M - K + 1,
      _N = N - K + 1;

    for (size_t y = 0; y < Y; y++) {
    for (size_t x = 0; x < X; x++) {
      ptr[y][x] = y + ((x * 1.0) / 100);
    }
  }
    */

    // Allocate local buffers (each get their own copy)
    printf(" %d on allocate \n", rank);
    if (rank != root) {
        input = malloc((myrest + down) * N * sizeof(float*));
        input[0] = malloc((myrest + down) * N * sizeof(float));
        for(int i = 0; i < (myrest + down); i++)
            input[i] = &input[0][i * N];  
    
    }
    printf(" %d on allocate \n", rank);
    // scatter input
    // printf("on scatterv \n");
    if (rank == root){
        for(int i=0; i < nprocs ; i++){
            printf("\t(%d)\t%d, %d \n", i, sendcounts[i], Sdispls[i]);
        }
    }
    printf("myrest: %d, down: %d \n", myrest, down);
    
    if (rank == 0){
        for (int i = 0; i < M; i++){
            for (int j = 0; j < N; j++){
                printf("%f", input[i][j]);
                }
            printf("\n");
        }
    }
    
    MPI_Scatterv(&(input[0][0]),                 // Sendbuff, matters only for root process.
                 sendcounts,
                 Sdispls,
                 MPI_FLOAT,
                 &(input[0][0]),                 // Recieve buff is local input array
                 (myrest + down) * N,
                 MPI_FLOAT,
                 root,
                 MPI_COMM_WORLD);
    
    
    printf("after scatterv \n");
    for (int r = 0; r < nprocs; r++){
        if (r == rank){
            printf("MY RANK: %d \n", rank);
            for (size_t i = 0; i < myrest; i++) {
                for (size_t j = 0; j < N; j++) {
                    printf(" %f ", input[i][j]);
                }
                printf("\n");
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    /*
    

    int i,j, ii, jj;
    double temp;
    for (i=0; i<=M-K; i++){
    for (j=0; j<=N-K; j++) 
    temp = 0.0;

    for (ii=0; ii<K; ii++)
    for (jj=0; jj<K; jj++)
    temp += input[i+ii][j+jj]*kernel[ii][jj];
    output[i][j] = temp;
    }
    */
   return;
}

int main(int argc, char *argv[])
{
int M=0, N=0, K=0, root =0, nprocs, rank; 

float **input=NULL, **output=NULL, **kernel=NULL;

MPI_Init (&argc, &argv);
MPI_Comm_rank (MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

if (rank==0) {
// read from command line the values of M, N, and K x
// allocate 2D array ’input’ with M rows and N columns
// allocate 2D array ’output’ with M-K+1 rows and N-K+1 columns x
// allocate the convolutional kernel with K rows and K columns x
// fill 2D array ’input’ with some values - x
// fill kernel with some values - x
// ....

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

    for(int i = 0; i< K; i++){
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
            kernel[i][j] = 0.01*i + 0.01*j;
            }
        }
}

// process 0 broadcasts values of M, N, K to all the other processes
MPI_Bcast(&M, 1, MPI_INT, root, MPI_COMM_WORLD);
MPI_Bcast(&N, 1, MPI_INT, root, MPI_COMM_WORLD);
MPI_Bcast(&K, 1, MPI_INT, root, MPI_COMM_WORLD);

if (rank>root) {
// allocate the convolutional kernel with K rows and K columns
float **kernel = malloc(K * (K) * sizeof(float*));
kernel[0] = malloc((K) * (K) * sizeof(float));

for(int i = 0; i < K; i++)
    kernel[i] = &(kernel[0][i * K]);
}

// process 0 broadcasts the content of kernel to all the other processes
MPI_Bcast(&kernel, 1, MPI_FLOAT, root, MPI_COMM_WORLD);

// parallel computation of a single-layer convolution
MPI_single_layer_convolution (M, N, input, K, kernel, output);

/*
if (rank==root) {
// For example, compare the content of array ’output’ with that is
// produced by the sequential function single_layer_convolution

// ...
}
*/
MPI_Finalize();
//free(input[0]);
//free(input);
return 0; 
}


// gr session: float **table     = (float **)malloc((*Y) *       sizeof(float *)); table[0]  = (float *) calloc((*Y) * (*X), sizeof(float));