#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <mpi.h>

#define DEFAULT_SIZE 16
#define MASTER_NODE 0
#define TRUE 1
#define FALSE 0

void printArray(int *, int);
int getMin(int, int);
int getMax(int, int);

void printArray(int *arrayToPrint, int arraySize) {
  printf("\n");
  int i;
  for(i = 0; i < arraySize; i++)
    printf("%d ", arrayToPrint[i]);
  printf("\n");
}

int getMin(int a, int b) {
  
  return (a < b) ? a : b;
}

int getMax(int a, int b) {

  return (a > b) ? a : b;
}

int main(int argc, char *argv[]) {
  // argv[1] = size of array

  if(argc != 2) {
    printf("Usage: ./<execfile> <array size>\n");
    return 1;
  }

  /* variables */
  int *listToSort,                            // Global list to sort
      arraySize,                              // In case of size != power of 2
      originalSize,                           // From user
      worldSize,                              // Number of nodes
      selfRank,                               // Node rank
      pairRank,                               // Node pair rank for compare and swap
      selfNameLength,                         // Node name length
      selfElement = -1,                       // The number held by the node
      pairElement = -1,                       // The number held by the node

      outerIteration,                         // Iteration on outer loop
      innerIteration,
      skipCount,                              // Increases every iteration
      increasing,                              // Determine whether increasing or no
      i, j;                                   // Iterator

  char selfName[MPI_MAX_PROCESSOR_NAME];      // Node name

  // Initialize MPI, get rank + name
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
  MPI_Comm_rank(MPI_COMM_WORLD, &selfRank);
  MPI_Get_processor_name(selfName, &selfNameLength);


  /* Master Node */
  if (selfRank == 0) {

    /* Get size on CLI */
    originalSize = atoi(argv[1]);
    arraySize = originalSize;

    /* Check if size is power of 2. If not, generate phantom slots/elements */
    while((arraySize & (arraySize - 1)) != 0)
      arraySize++;

    /* Initialize array */
    listToSort = (int *) malloc(arraySize * sizeof(int));
    for(i = 0; i < arraySize; i++)
      // listToSort[i] = (i >= originalSize) ? INT_MAX : rand() % arraySize + 1;
      listToSort[i] = (i >= originalSize) ? INT_MAX : i;
  }

  /* Broadcast original and array size */
  MPI_Bcast(&originalSize, 1, MPI_INT, MASTER_NODE, MPI_COMM_WORLD);
  MPI_Bcast(&arraySize, 1, MPI_INT, MASTER_NODE, MPI_COMM_WORLD);

  /* Scatter all elements to nodes */
  MPI_Scatter(listToSort, 1, MPI_INT, &selfElement, 1, MPI_INT, MASTER_NODE, MPI_COMM_WORLD);
  
  /* Pre-processing, make list bitonic */
  /* Outer loop has an iteration of log(size) - 1 */
  outerIteration = (int) log2(arraySize) - 1; 
  skipCount = 1;
  innerIteration = 1;
  printf("\n================ RANK %d ================ \n", selfRank);



  for(i = 0; i < outerIteration; i++) {
    
    skipCount = (int) pow(2, i);

    for(j = 0; j < innerIteration; j++) {
        printf("Current i j: %d %d\n", i, j);
        if(selfRank % 2 == 0) {
          MPI_Send(&selfElement, 1, MPI_INT, selfRank + skipCount, 0, MPI_COMM_WORLD);
          MPI_Recv(&pairElement, 1, MPI_INT, selfRank + skipCount, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          printf("Received from %d: %d\n", selfRank + skipCount, pairElement);
        }

        else {
          MPI_Recv(&pairElement, 1, MPI_INT, selfRank - skipCount, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          printf("Received from %d: %d\n", selfRank - skipCount, pairElement);
          MPI_Send(&selfElement, 1, MPI_INT, selfRank - skipCount, 0, MPI_COMM_WORLD);
        }
    
    }

    innerIteration++;

  }





  // printf("Self rank, element, original size: %d  %d  %d\n", selfRank, selfElement, originalSize);
  MPI_Finalize();
  return 0;
}