#include <stdlib.h>
#include <stdio.h>

void read_graph_from_file2();

int main(int argc, char const *argv[]) {
  int N;
  int *row_ptr;
  int *col_idx;
  read_graph_from_file2(("facebook.txt"), &N, &row_ptr, &col_idx);
  free(row_ptr);
  free(col_idx);
  return 0;
}

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

  //lager kopier
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
for (size_t i =0; i<E; i++){
if (i >= (*row_ptr)[node +1]){
  node++;
}
  (*col_idx)[tmp_row_ptr2[row_idx[i]]] = node;
  tmp_row_ptr2[row_idx[i]]++;
}

for (size_t i=0;i<50;i++){
printf("col_idx %d \n", (*col_idx)[i]);
//printf("row_idx %d \n", (row_idx)[i]);
}
  fclose(fp);
  free(countNeighbours);
  free(toNode);
  free(fromNode);
  free(tmp_row_ptr);
  free(tmp_row_ptr2);
  return;
}

//printf("row_ptr %d %d \n", (*row_ptr)[0], (*row_ptr)[1]);
//printf("%d %d", (*col_idx)[0], (*col_idx)[1]);
// row_ptr[k+1]-row_ptr[k]????
/*
Hint: The most efficient strategy (at least with respect to temporary memory usage) for building up a CRS-formatted hyperlink matrix is to read the web
graph file twice. The first pass is only used to “count” the various information,
such as the info needed to set up the array row ptr (in particular, how many
inbound links each webpage has) and the number of links excluding all self-links
(if any). It is in the second pass that the values in arrays col idx and val are
filled.
*/
/*
if (tmp_row_ptr[i] != tmp_row_ptr[i+1]){
  (*col_idx)[tmp_row_ptr[i]] = toNode[tmp_row_ptr[i]];
  tmp_row_ptr[i]++;

  if (tmp_row_ptr[i] == tmp_row_ptr[i+1]){
    continue;
  }
*/
