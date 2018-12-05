#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <mpi.h>


#define DEF_SIZE 16

int main(int argc, char *argv[]){
	MPI_Init(&argc, &argv);

	int *matrix;
	     matrixsize, powersize,
	     i,j,k,l,m,

	     ownnumber, pairnumber,

	     maxpower1, preprocessorouterloop, preprocessorinnerloop,

	     csize, crank, cnamelen;
	char cname[MPI_MAX_PROCESSOR_NAME];

	// Fetch matrix size, and power size
	if(argc == 2) matrixsize=strtol(argv[1],NULL,10);
	else matrixsize= DEF_SIZE;
	while(1){
		maxpower=1;
		if(matrixsize<=pow(2,maxpower)){
			powersize=pow(2,maxpower);
			break;	
		}
		maxpower++;
	}
	preprocessorouterloop = maxpower-1; // hindi to nagbabago
	preprocessorinnerloop = 1;	//nagiincrement by one
	preprocessorinnerinnerloop = preprocessorinnerloop; //decrement by one

	matrix = malloc(powersize*sizeof(*matrix));
	for(i=0;i<powersize;i++){
		if(i<matrixsize) matrix[i] = rand()%matrixsize+1;
		else matrix[i] = INT_MAX;
	}

/***************************************************************************
** PREPROCESSING
***************************************************************************/






	MPI_Comm_size(			&csize, MPI_COMM_WORLD);
	MPI_Comm_rank(			&crank, MPI_COMM_WORLD);
	MPI_Get_processor_name(	cname, 	cnamelen);

}