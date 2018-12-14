#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <mpi.h>

#define DEFAULT_SIZE 16
#define MASTER_NODE 0

void printArray(int *, int);

void printArray(int *arrayToPrint, int arraySize) {
  printf("\n");
  int i;
  for(i = 0; i < arraySize; i++)
    printf("%d ", arrayToPrint[i]);
  printf("\n");
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
      selfNameLength,                         // Node name length
      selfElement = -1,                       // The number held by the node
      i;                                      // Iterator

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
      listToSort[i] = (i >= originalSize) ? INT_MAX : rand() % arraySize + 1;
    
  }

  /* SCATTER! :> */
  MPI_Scatter(listToSort, 1, MPI_INT, &selfElement, 1, MPI_INT, MASTER_NODE, MPI_COMM_WORLD);
  
















  printf("Self rank, element: %d  %d\n", selfRank, selfElement);
  MPI_Finalize();
  return 0;
}