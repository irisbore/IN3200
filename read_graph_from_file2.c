#include <stdlib.h>
#include <stdio.h>

void read_graph_from_file2(char *filename, int *N, int **row_ptr,
int **col_idx){

  FILE * fp;
  fp = fopen(filename, "r");
  if (fp == NULL){
        printf("Could not open file %s",filename);
      }
  fscanf(fp, "%*[^\n]\n");
  fscanf(fp, "%*[^\n]\n");
  int E;
  fscanf(fp, "%*s %*s %d %*s %d \n", &(*N), &E);
  fscanf(fp, "%*[^\n]\n");

  int *fromNode = (int*) calloc(E, sizeof(int));
  int *toNode = (int*) calloc(E, sizeof(int));

  int tmp1, tmp2;
  for (size_t i = 0; i < E; i ++){
    fscanf(fp, "%d %d", &tmp1, &tmp2);
    if (tmp1 != tmp2){
    fromNode[i] = tmp1;
    toNode[i] = tmp2;
    }
  }

int *countNeighbours = (int*) calloc(E, sizeof(int));
for (size_t i = 0; i < E; i ++){
  countNeighbours[fromNode[i]] ++;
  countNeighbours[toNode[i]] ++;
}

*row_ptr =  (int*) calloc((*N+1), sizeof(int));
int *tmp_row_ptr = (int*) calloc((*N+1), sizeof(int));
int *tmp_row_ptr2 = (int*) calloc((*N+1), sizeof(int));

(*row_ptr)[1] = countNeighbours[0];
tmp_row_ptr[1] = (*row_ptr)[1];
tmp_row_ptr2[1] = (*row_ptr)[1];

for (size_t i=2;i<(*N+1);i++){
  (*row_ptr)[i] = countNeighbours[i-1] + (*row_ptr)[i-1];

  //create copies efficiently
  tmp_row_ptr[i] = (*row_ptr)[i];
  tmp_row_ptr2[i] = (*row_ptr)[i];
}


*col_idx = (int*) calloc(2 * E, sizeof(int));
int *row_idx = (int*) calloc(2 * E, sizeof(int));


for (size_t i =0; i<E; i++){
  row_idx[tmp_row_ptr[fromNode[i]]] = toNode[i];
  tmp_row_ptr[fromNode[i]]++;

  row_idx[tmp_row_ptr[toNode[i]]] = fromNode[i];
  tmp_row_ptr[toNode[i]]++;
}

int node = 0;
for (size_t i =0; i< 2*E; i++){ //remove 2?
if (i >= (*row_ptr)[node +1]){
  node++;
}
  (*col_idx)[tmp_row_ptr2[row_idx[i]]] = node;
  tmp_row_ptr2[row_idx[i]]++;
}

  fclose(fp);
  free(countNeighbours);
  free(toNode);
  free(fromNode);
  free(tmp_row_ptr);
  free(tmp_row_ptr2);
  return;
}
