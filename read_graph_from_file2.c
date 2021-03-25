#include <stdlib.h>
#include <stdio.h>

void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx){

  FILE *fp  = fopen(filename, "r");
  if (fp == NULL){
        printf("Could not open file %s",filename);
      }
  fscanf(fp, "%*[^\n]\n");
  fscanf(fp, "%*[^\n]\n");
  int E;
  fscanf(fp, "%*s %*s %d %*s %d \n", &(*N), &E);
  fscanf(fp, "%*[^\n]\n");
  
  int *fromNode = (int*) malloc(E * sizeof(int));
  int *toNode = (int*) malloc(E * sizeof(int));
  int *countNeighbours = (int*) calloc(*N, sizeof(int));
  int tmp1, tmp2;
  int tmpE = E;

  for (size_t i = 0; i < tmpE; i ++){
    fscanf(fp, "%d %d", &tmp1, &tmp2);
    if (tmp1 != tmp2 && 0 <= tmp1 < *N && 0 <= tmp2 < *N ){
    fromNode[i] = tmp1;
    toNode[i] = tmp2;
    countNeighbours[fromNode[i]] ++;
    countNeighbours[toNode[i]] ++;
    }
    else{
      i--;
      tmpE--;
    }
  }
  fclose(fp);
  

*row_ptr =  (int*) calloc((*N+1), sizeof(int));
int *tmp_row_ptr = (int*) calloc((*N+1), sizeof(int));
int *tmp_row_ptr2 = (int*) calloc((*N+1), sizeof(int));

for (size_t i = 1; i < *N+1; i++){
  (*row_ptr)[i] = countNeighbours[i-1] + (*row_ptr)[i-1];

  //create copies efficiently
  tmp_row_ptr[i] = (*row_ptr)[i];
  tmp_row_ptr2[i] = (*row_ptr)[i];
}

*col_idx = (int*) malloc(2 * E * sizeof(int));
int *row_idx = (int*) malloc(2 * E * sizeof(int));


for (size_t i =0; i<E; i++){
  row_idx[tmp_row_ptr[fromNode[i]]] = toNode[i];
  tmp_row_ptr[fromNode[i]]++;

  row_idx[tmp_row_ptr[toNode[i]]] = fromNode[i];
  tmp_row_ptr[toNode[i]]++;
}

int node = 0;
for (size_t i =0; i < 2*E; i++){ 
  if (i >= (*row_ptr)[node +1]){
    node++;
  }
  (*col_idx)[tmp_row_ptr2[row_idx[i]]] = node;
  tmp_row_ptr2[row_idx[i]]++;
}
  free(toNode);
  free(fromNode);
  free(countNeighbours);
  free(row_idx);
  free(tmp_row_ptr);
  free(tmp_row_ptr2);
  return;
}
