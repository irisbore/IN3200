#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

void create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val) {
  *SNN_val = (int*) calloc(row_ptr[N], sizeof(int));
  #pragma omp parallel for 
  for (size_t i=0; i<N; i++){
    int *traverse_i = &col_idx[row_ptr[i]];
    int i_range = row_ptr[i+1] - row_ptr[i];
        for (size_t k=row_ptr[i]; k<row_ptr[i+1]; k++){
            int j = col_idx[k];

            int *traverse_j = &col_idx[row_ptr[j]];
            int i_current=0, j_current=0;
            int j_range = row_ptr[j+1] - row_ptr[j];

            while (i_current < i_range && j_current < j_range){
                if (traverse_i[i_current] < traverse_j[j_current]){
                    i_current++;
                }
                else if (traverse_j[j_current] < traverse_i[i_current]){
                    j_current++;
                }
            
                else{ 
                    (*SNN_val)[k]++;
                    i_current++;
                    j_current++;
                }   
            }
        }
    }
  return;
}