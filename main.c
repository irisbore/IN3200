#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "read_graph_from_file1.h"
#include "read_graph_from_file2.h"
#include "create_SNN_graph1.h"
#include "create_SNN_graph2.h"
#define CLOCKS_TO_MILLISEC(t) (t*1000)/CLOCKS_PER_SEC

int main(int argc, char const *argv[]) {
  int N;
  char **table2D;
  clock_t start_serial, time;
  printf("Reading graph 1 \n");
  start_serial = clock();
  read_graph_from_file1(("facebook_combined.txt"), &N, &table2D);
  time = clock() - start_serial;
  printf("Running time is %lu ms \n", CLOCKS_TO_MILLISEC(time));

  printf("Reading graph 2 \n");
  int *row_ptr;
  int *col_idx;

  start_serial = clock();
  read_graph_from_file2(("facebook_combined.txt"), &N, &row_ptr, &col_idx);
  time = clock() - start_serial;
  printf("Running time is %lu ms\n", CLOCKS_TO_MILLISEC(time));
  
  printf("Creating SNN \n");
  int **SNN_table;
  start_serial = clock();
  create_SNN_graph1(N, table2D, &SNN_table);
  time = clock() - start_serial;

  printf("Running time SNN1 in serial is %lu ms\n", CLOCKS_TO_MILLISEC(time));

  printf("Creating SNN_val \n");
  int *SNN_val;
  start_serial = clock();
  create_SNN_graph2(N, row_ptr, col_idx, &SNN_val);
  time = clock() - start_serial;
  printf("Running time is %lu ms\n", CLOCKS_TO_MILLISEC(time));

  printf("Testing Create_snn_val \n");

  //for (int i=0;i<30;i++){
    //printf("%d \n", SNN_val[i]);
  //}

#ifdef _OPENMP
#include <omp.h>
  double start_par, t;
  start_par = omp_get_wtime();
  create_SNN_graph1(N, table2D, &SNN_table);
  t = omp_get_wtime() - start_par;
  printf("Running time SNN1 in parallell is %f ms \n", t*1000);
  
#endif
  free(table2D);
  free((table2D)[0]);
  free(row_ptr);
  free(col_idx);
  free(SNN_table);
  free(SNN_table[0]);
  free(SNN_val);

return 0;
}