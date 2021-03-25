serial: read_graph_from_file1.o read_graph_from_file2.o serial.o
	gcc  -fopenmp main.o read_graph_from_file1.o read_graph_from_file2.o create_SNN_graph1.o create_SNN_graph2.o -o serial

parallel: read_graph_from_file1.o read_graph_from_file2.o parallel.o
	gcc -fopenmp main.o read_graph_from_file1.o read_graph_from_file2.o create_SNN_graph1.o create_SNN_graph2.o -o parallel

serial.o: main.c create_SNN_graph1.c create_SNN_graph2.c 
	gcc -c main.c create_SNN_graph1.c create_SNN_graph2.c 

parallel.o: main.c create_SNN_graph1.c create_SNN_graph2.c 
	gcc -fopenmp -c main.c create_SNN_graph1.c create_SNN_graph2.c 

read_graph_from_file1.o: read_graph_from_file1.c 
	gcc -c read_graph_from_file1.c 

read_graph_from_file2.o: read_graph_from_file2.c
	gcc -c read_graph_from_file2.c

clean:
	rm -f *.o serial parallel a.out vgcore.*

