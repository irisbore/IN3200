#include <stdlib.h>
#include <stdio.h>

void read_graph_from_file1(char *filename, int *N, char ***table2D){
  FILE * fp;
  fp = fopen(filename, "r");
  if (fp == NULL){
        printf("Could not open file %s", filename);
      }

  fscanf(fp, "%*[^\n]\n");
  fscanf(fp, "%*[^\n]\n");

  int E;
  fscanf(fp, "%*s %*s %d %*s %d \n", &(*N), &E);

  fscanf(fp, "%*[^\n]\n");

  *table2D = (char **) calloc(*N, sizeof(char*));

  (*table2D)[0] = (char*) calloc(*N * *N, sizeof(char));

  for (size_t i=1; i< (*N); i++){
    (*table2D)[i] = &(*table2D)[0][i * (*N)];
  }

  int fromNode;
  int toNode; 
  for (size_t i = 0; i < E; i ++){
    fscanf(fp, "%d %d", &fromNode, &toNode);
    if (fromNode != toNode && 0 <= fromNode < *N && 0 <= toNode < *N  ) {
    (*table2D)[fromNode][toNode] = 1;
  }
  }
  fclose(fp);
  return;
}


