
a.out: main.o read_graph_from_file1.o create_SNN_graph1.o read_graph_from_file2.o create_SNN_graph2.o
	gcc -fopenmp -o a.out main.o read_graph_from_file1.o create_SNN_graph1.o read_graph_from_file2.o create_SNN_graph2.o

main.o: main.c
	gcc -c main.c -fopenmp
	
read_graph_from_file1.o: read_graph_from_file1.c
	gcc -c read_graph_from_file1.c 

read_graph_from_file2.o: read_graph_from_file2.c
	gcc -c read_graph_from_file2.c 

create_SNN_graph1.o: create_SNN_graph1.c
	gcc  -c create_SNN_graph1.c -fopenmp

create_SNN_graph2.o: create_SNN_graph2.c
	gcc -c create_SNN_graph2.c 

clean:
	rm -f *.o a.out
