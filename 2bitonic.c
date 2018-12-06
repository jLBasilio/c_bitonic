#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <mpi.h>

#define DEF_SIZE 16
#define ROOT 0

int main(int argc, char *argv[]){
    MPI_Init(&argc,&argv);

    int *matrix,
        msize, psize,
        i=0,j,k,l,m,n,

    //Preprocessing variables
        maxpower=0, mainpower=1,
        members, sranks,

    //Slave variables
        ownnum, pairnum,
        ascending,

    //MPI variables
        csize,crank,cnamelen;
    char cname[MPI_MAX_PROCESSOR_NAME];

    //Fetch MPI details
    MPI_Comm_size(MPI_COMM_WORLD,&csize);
    MPI_Comm_rank(MPI_COMM_WORLD,&crank);
    MPI_Get_processor_name(cname,&cnamelen);

    printf("%s rank:%d\n", cname, crank);

    //Root procedures
    if(crank==ROOT){
        //Fetch matrix size, power size
        if(argc>=2) msize=strtol(argv[1],NULL,10);
        else msize=DEF_SIZE;
        while(++maxpower){
            if(msize<=pow(2,i)){
                psize = pow(2,i);
                break;
            } 
        }
        //Fetch whether ascending or descending sort
        if(argc>=3) ascending=strtol(argv[2],NULL,10);
        else ascending = 1;

        //generate matrix in power size
        matrix = malloc(psize*sizeof(*matrix));
        for(i=0;i<psize;i++) matrix[i] = (i<msize) ? rand()%msize+1 : 10000000;
    }

    // Distribute matrix to slaves' ownnum
    MPI_Scatter(matrix,psize,MPI_INT,&ownnum,1,MPI_INT,ROOT,MPI_COMM_WORLD);
    // Distribute maxpower to slaves
    MPI_Bcast(&maxpower,1,MPI_INT,ROOT,MPI_COMM_WORLD);

    printf("Checkpoint\n");

/*******PREPROCESSING************************************************************/
    
    //Outer Loop for log(p)-1
    mainpower=1;
    for(i=0;i<maxpower-1;i++){
        //Inner loop for mainpower
        for(j=mainpower;j>=0;j--){
            members = pow(2,j);
            sranks = members/2;
            //Determine whether slave is source or destination in pair
            if(crank%members<sranks){
                MPI_Send(&ownnum,1,MPI_INT,crank+pow(2,j-1),1,MPI_COMM_WORLD);
                MPI_Recv(&pairnum,1,MPI_INT,crank+pow(2,j-1),1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            } 
            else{
                MPI_Recv(&pairnum,1,MPI_INT,crank-pow(2,j-1),1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                MPI_Send(&ownnum,1,MPI_INT,crank-pow(2,j-1),1,MPI_COMM_WORLD);
            }
            if(((int)floor(crank/members)%2 && ascending) || ((int)floor(crank/members)%2 && !ascending)) ownnum = (ownnum < pairnum) ? ownnum : pairnum;
            else ownnum = (ownnum < pairnum) ? pairnum : ownnum;

        }
        mainpower++;
    }







    MPI_Finalize();
}