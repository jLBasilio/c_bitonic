reset
mpicc -o new_bitonic new_bitonic.c -lm && mpirun -np 16 ./new_bitonic 16