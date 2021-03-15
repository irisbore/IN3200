/*
2.1 Reading a connectivity graph from file
You are requested to implement two functions with the following syntax:
• void read graph from file1 (char *filename, int *N, char ***table2D)
which reads a text file (filename prescribed as input) that contains a connectivity
graph, outputs the number nodes through N, allocates a 2D table of correct dimension inside the function, fills its content and passes it out (thus triple pointer
as type for table2D).
*/
#include <stdlib.h>
#include <stdio.h>

void read_graph_from_file1();
void create_SNN_graph1();

int main(int argc, char const *argv[]) {
  int N;
  char **table2D;
  read_graph_from_file1(("simple-graph.txt"), &N, &table2D);
  int **SNN_table;
  create_SNN_graph1(N, table2D, &SNN_table);
  free(table2D);
  free((table2D)[0]);
  free(SNN_table);
  free((SNN_table)[0]);
  return 0;
}

void read_graph_from_file1(char *filename, int *N, char ***table2D){
  //Reads in table format
  FILE * fp;
  fp = fopen(filename, "r");
  if (fp == NULL){
        printf("Could not open file %s",filename);
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

  //Assign values to table
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

void create_SNN_graph1(int N, char **table2D, int ***SNN_table){
  *SNN_table = (int **) calloc(N, sizeof(int*));
  (*SNN_table)[0] = (int*) calloc(N * N, sizeof(int));

  for (size_t i=1; i<N; i++){
    (*SNN_table)[i] = &(*SNN_table)[0][i * N];
  }

  int countSNN;
  for (size_t i=0; i<N; i++){
    for (size_t j=i+1; j<N; j++){
      if (table2D[i][j] == 1){
        countSNN = 0;
        for (size_t k=0; k<N; k++){
          if(table2D[i][k] && (table2D[j][k])){
            countSNN ++;
            (*SNN_table)[i][j] = countSNN;
            (*SNN_table)[j][i] = countSNN;
          }
        }
        }
      }
    }
    for (int i=0;i<N;i++){
      for (int j=0;j<N;j++){
        printf("%d \n", (*SNN_table)[i][j]);
    }
    }
  return;
}
