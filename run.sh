reset
mpicc -o new_bitonic new_bitonic.c
mpirun -np 16 ./new_bitonic 16