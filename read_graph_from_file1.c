#include <stdlib.h>
#include <stdio.h>

void read_graph_from_file1(char *filename, int *N, char ***table2D){
  //Reads in table format
  FILE * fp;
  fp = fopen(filename, "r");
  if (fp == NULL){
        printf("Could not open file %s", filename);
      }

  //skip first two lines
  fscanf(fp, "%*[^\n]\n");
  fscanf(fp, "%*[^\n]\n");

  int E;
  fscanf(fp, "%*s %*s %d %*s %d \n", &(*N), &E);

  //skip fourth line
  fscanf(fp, "%*[^\n]\n");

  //if ***table2D is triple-pointed, we strip the calloc of two **
  //sets first row to N zeros
  //(char **) typecasts pointer returned by calloc
  *table2D = (char **) calloc(*N, sizeof(char*));

  (*table2D)[0] = (char*) calloc(*N * *N, sizeof(char));

  //Initialize table
  for (size_t i=1; i< (*N); i++){
    (*table2D)[i] = &(*table2D)[0][i * (*N)];
  }

  //Number of edges is the same as remaining number of lines
  int fromNode;
  int toNode; 
  for (size_t i = 0; i < E; i ++){
    fscanf(fp, "%d %d", &fromNode, &toNode);
    if (fromNode != toNode) {
    (*table2D)[fromNode][toNode] = 1;
  }
  }
  fclose(fp);
  return;
}


