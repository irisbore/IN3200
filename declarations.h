void read_graph_from_file1(char *filename, int *N, char ***table2D);
void read_graph_from_file2(char *filename, int *N, int **row_ptr, int **col_idx);
void create_SNN_graph1(int N, char **table2D, int ***SNN_table);
void create_SNN_graph2(int N, int *row_ptr, int *col_idx, int **SNN_val);