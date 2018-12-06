#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>



int main() {


  int arrayOfInts[] = {3, 5, 8, 9, 10, 12, 14, 20, 95, 60, 40, 35, 23, 18, 0};
  int arrayLen = 16;

  int noIterations = (int) log2(arrayLen);

  int i, j, halfLen, temp, jumpIndex;

  halfLen = arrayLen/2;


  for(i = 0; i < arrayLen; i++)
    printf("%d ", arrayOfInts[i]);
  printf("\n");


  jumpIndex = halfLen;
  for(i = 0; i < noIterations; i++) {

    j = 0;
    while((j + pairIndex) != halfLen - 1) {


      


      
    }
  jumpIndex /= 2;

  }


  for(i = 0; i < arrayLen; i++)
    printf("%d ", arrayOfInts[i]);
  printf("\n");


  return 0;


}



