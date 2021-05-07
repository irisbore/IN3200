MPIFILES = main.c MPI_single_layer_convolution.c set.h 

main: $(MPIFILES)
	/usr/lib64/openmpi/bin/mpicc $(MPIFILES) -o main

run:
	/usr/lib64/openmpi/bin/mpirun -np 24 ./main 1000 1000 9

clean:
	rm -f *.o main a.out vgcore.*