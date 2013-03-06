//
//  main.c
//  First
//
//  Created by Alexey Goncharov on 22.02.13.
//  Copyright (c) 2013 DIHT. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, const char * argv[])
{
  int *aArray, *bArray, *cArray;
//  printf("enter n\n");
  int n = 0, i = 0, sum = 0;
  
 // scanf("%d", &n);
  n = 100000000;
  printf("malloc started\n");
  time_t currentTime = 0;
  currentTime = time(NULL);
  
  aArray = (int *)malloc(n*sizeof(int));
  bArray = (int *)malloc(n*sizeof(int));
  cArray = (int *)malloc(n*sizeof(int));
  printf("allocated %ld\n", time(NULL) - currentTime);

  for (i = 0; i < n; ++i) {
    aArray[i] = i;
    bArray[i] = i;
  }
  printf("filled %ld\n", time(NULL) - currentTime);

//  printf("%ld\n", currentTime);
  for (i = 0; i < n; ++i) {
    cArray[i] = aArray[i] + bArray[i];
    sum += cArray[i];
  }
  printf("summed %ld\n", time(NULL) - currentTime);
  
  
  currentTime = time(NULL);
  printf("calloc started\n");

  
  aArray = (int *)calloc(sizeof(int), n);
  bArray = (int *)calloc(sizeof(int), n);
  cArray = (int *)calloc(sizeof(int), n);
  printf("allocated %ld\n", time(NULL) - currentTime);
  
  for (i = 0; i < n; ++i) {
    aArray[i] = i;
    bArray[i] = i;
  }
  printf("filled %ld\n", time(NULL) - currentTime);
  
//  printf("%ld\n", currentTime);
  for (i = 0; i < n; ++i) {
    cArray[i] = aArray[i] + bArray[i];
    sum += cArray[i];
  }
  printf("summed %ld\n", time(NULL) - currentTime);
  
  return 0;
}

