#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "declarations.h"
#include <omp.h>

int main(int argc, char const *argv[]) {
  int N;
  char **table2D;
  int *col_idx;
  int *row_ptr;
  int **SNN_table;
  int *SNN_val;
  double start, time_read1, time_read2, time_SNN1, time_SNN2;
    
    printf("Testing read_graph_from_file1 \n");
    start = omp_get_wtime();
    read_graph_from_file1("facebook_combined.txt", &N, &table2D);
    time_read1= omp_get_wtime() - start;

    printf("Testing read_graph_from_file2 \n");
    start = omp_get_wtime();
    read_graph_from_file2("facebook_combined.txt", &N, &row_ptr, &col_idx);
    time_read2 = omp_get_wtime() - start;

    printf("Testing create_SNN_graph1 \n");
    start = omp_get_wtime();
    create_SNN_graph1(N, table2D, &SNN_table);
    time_SNN1 = omp_get_wtime() - start;

    printf("Testing create_SNN_graph2 \n");
    start = omp_get_wtime();
    create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);
    time_SNN2 = omp_get_wtime() - start;
    
  //printf(Run time is %f, time_)

  free(table2D);
  free((table2D)[0]);
  free(row_ptr);
  free(col_idx);
  free(SNN_table);
  free(SNN_table[0]);
  free(SNN_val);
return 0;
}