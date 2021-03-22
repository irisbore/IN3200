#include <stdlib.h>
#include <stdio.h>
#include "read_graph_from_file2.h"

void create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val){

*SNN_val = (int*) calloc(row_ptr[N], sizeof(int));
int c;
for (size_t i=0; i<N; i++){ //N-1?
  for (size_t j=0;j<N-1; j++){ 
    c = col_idx[j];

    int *line_i = &col_idx[row_ptr[i]];
    int *line_j = &col_idx[row_ptr[j]];
    int i_idx = 0;
    int j_idx = 0;

    int i_trm = row_ptr[i+1] - row_ptr[i];
    int j_trm = row_ptr[c+1] - row_ptr[c];

    while(i_idx < i_trm && j_idx < j_trm){
      if (line_i[i_idx] < line_j[j_idx]){
        i_idx++;
      } else if (line_j[j_idx] < line_i[i_idx]) {
        j_idx++;
      } else {
        (*SNN_val)[j]++;
        (*SNN_val)[row_ptr[j] + j_idx]++;
        i_idx++;
        j_idx++;
      }
    }

  }
}
}
