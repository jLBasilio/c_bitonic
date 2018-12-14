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

  int *vector,
  msize, psize,
  i=0,j,k,l,m,n,

  //Preprocessing variables
  maxpower=0, mainpower=1,
  members, issrc, prank,isplus,

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

    //Fetch vector size, power size
    msize = (argc >= 2) ? strtol(argv[1], NULL, 10) : DEF_SIZE;

    while(++maxpower){
      printf("maxpower: %d\n",maxpower);
      if(msize <= pow(2, maxpower)){
        psize = pow(2, maxpower);
        break;
      }
    } 

    //Fetch whether ascending or descending sort
    ascending = (argc >= 3) ? strtol(argv[2], NULL,10) : 1;

    //generate vector in power size
    vector = malloc(psize*sizeof(*vector));
    for(i=0;i<psize;i++) vector[i] = (i<msize) ? rand()%msize+1 : INT_MAX;

    for(i=0;i<psize;i++){
        printf("%d ", vector[i]);
    }
    printf("\n\n");
  }

    // Distribute vector to slaves' ownnum
    MPI_Scatter(vector,1,MPI_INT,&ownnum,1,MPI_INT,ROOT,MPI_COMM_WORLD);
    
    // Distribute maxpower to slaves
    MPI_Bcast(&maxpower,1,MPI_INT,ROOT,MPI_COMM_WORLD);


/*******PREPROCESSING************************************************************/
    
    //Outer Loop for log(p)-1
    mainpower=1;
    for(i=0;i<maxpower-1;i++){
        //Inner loop for mainpower
        for(j=mainpower;j>=0;j--){
            members = pow(2,j);
            issrc   = (crank%members<members/2)?1:0;
            prank   = (issrc)?crank+pow(2,j-1):crank-pow(2,j-1);
            isplus  = ((!(int)floor(crank/members)%2 && ascending) || ((int)floor(crank/members)%2 && !ascending)) ? 1:0;
            
            //Determine whether slave is source or destination in pair
            if(issrc){
                MPI_Send(&ownnum,1,MPI_INT,crank+pow(2,j-1),1,MPI_COMM_WORLD);
                MPI_Recv(&pairnum,1,MPI_INT,crank+pow(2,j-1),1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            } 
            else{
                MPI_Recv(&pairnum,1,MPI_INT,crank-pow(2,j-1),1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
                MPI_Send(&ownnum,1,MPI_INT,crank-pow(2,j-1),1,MPI_COMM_WORLD);
            }
            if(((int)floor(crank/members)%2 && ascending) || ((int)floor(crank/members)%2 && !ascending)) ownnum = (ownnum < pairnum) ? ownnum : pairnum;
            else ownnum = (ownnum < pairnum) ? pairnum : ownnum;
            
            printf("%s\tcrank:%2d prank:%2d shdplus:%2d isplus:%2d i:%d j%d\n",cname,crank,prank,(int)floor(crank/members)%2,isplus,i,j);
        }
        mainpower++;
    }


    MPI_Gather(&ownnum, 1, MPI_INT, vector,1,MPI_INT,ROOT,MPI_COMM_WORLD);

    if(crank==ROOT){
        for(i=0;i<psize;i++){
            printf("%d ", vector[i]);
        }
        printf("\n\n");
        free(vector);
    }

/*****SORTING PROPER*************************************************************/

    MPI_Finalize();
    return 1;
}