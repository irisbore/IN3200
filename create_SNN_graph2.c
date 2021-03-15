#include <stdlib.h>
#include <stdio.h>

void read_graph_from_file2();

int main(int argc, char const *argv[]) {
  return 0;
}

void_create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val){
  E = row_ptr[N]
  *SNN_val = (int*) calloc(2*E, sizeof(int));

  for (i=1; i<N+1; i++);
  {
    row_ptr[i] = row_ptr[i] + row_ptr[i]
  }

  /* The CRS format for an SNN graph must use an additional array containing non-negative integer values, named such as SNN val,
which is of length 2Nedges (same as col idx) */

}
