#include <stdlib.h>
#include <stdio.h>
#include <omp.h> 
#include "read_graph_from_file1.h"
#include "read_graph_from_file2.h"
#include "create_SNN_graph1.h"
#include "create_SNN_graph2.h"

int main(int argc, char const *argv[]) {
  int N;
  char **table2D;
  read_graph_from_file1(("simple-graph.txt"), &N, &table2D);

  int *row_ptr;
  int *col_idx;
  read_graph_from_file2(("facebook.txt"), &N, &row_ptr, &col_idx);


  int **SNN_table;
  create_SNN_graph1(N, table2D, &SNN_table);

  free(table2D);
  free((table2D)[0]);
  free(row_ptr);
  free(col_idx);
  free(SNN_table);
  free((SNN_table)[0]);
  return 0;
}