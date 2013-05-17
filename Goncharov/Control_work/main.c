//
//  main.c
//  Thread_function
//
//  Created by Alexey Goncharov on 10.05.13.
//  Copyright (c) 2013 DIHT. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>
#include <malloc/malloc.h>

#define debugState 0

pthread_mutex_t lock, lock1, lock2, lock3, lock4;

void *function(void *data) {
  int funcID = 0;
  pthread_mutex_lock(&lock);
  funcID = ((int **)data)[6][0]++;
  if(debugState) printf("function %d started\n", funcID);
  int *numbers = ((int **)data)[0];
  int *queue = ((int **)data)[funcID];
  int *nextQueue = NULL;
  if (funcID <= 5) nextQueue = ((int **)data)[funcID + 1];
  pthread_mutex_unlock(&lock);
  int done = 0;
  int n = 10;
  while (1) {
    if (done == n) {break;}
    for (int i = 0; i < n; ++i) {
      pthread_mutex_lock(&lock4);
      if (queue[i] == 1) {
        if(debugState) printf("changing element at index %d\n", i);
        queue[i] = 0;
        pthread_mutex_unlock(&lock4);
        
        pthread_mutex_lock(&lock2);
        numbers[i] += funcID;
        pthread_mutex_unlock(&lock2);

        if (nextQueue) {
          pthread_mutex_lock(&lock3);
          nextQueue[i] = 1;
          pthread_mutex_unlock(&lock3);
        }
        ++done;
      } else pthread_mutex_unlock(&lock4);
    }
  }
  if(debugState) printf("function %d\n", funcID);

  return NULL;
}



// array of numbers
// 5 arrays of tasks for each of function


int main(int argc, const char * argv[])
{
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_mutex_init(&lock, NULL);
  pthread_mutex_init(&lock1, NULL);
  pthread_mutex_init(&lock2, NULL);
  pthread_mutex_init(&lock3, NULL);
  pthread_mutex_init(&lock4, NULL);

  
  pthread_t *threads = (pthread_t *)malloc(5*sizeof(pthread_t));
  
  int **array = (int **)malloc(7*sizeof(int *));
  int n = 10;
  for (int i = 0; i < 7; ++i) {
    array[i] = (int *)malloc(n*sizeof(int));
    
    for (int j = 0; j < n; ++j) {
      if (i == 0) {
        array[i][j] = j;
        continue;
      }
      if (i == 1) {
        array[i][j] = 1;
        continue;
      }
      if (i == 6) {
        array[i][j] = j == 0 ? 1 : 0;
        continue;
      }
      array[i][j] = 0;
    }
  }
  for (int i = 0; i < 7; ++i) {
    for (int j = 0; j < n; ++j) {
      printf("%d\t", array[i][j]);
    }
    printf("\n");
  }
  for (int i = 0; i < 5; ++i) {
    pthread_create(&threads[i], &attr, function , array);
  }
  
  for (int i = 0; i < 5; ++i) {
    pthread_join(threads[i], NULL);
  }
  for (int i = 0; i < 7; ++i) {
    for (int j = 0; j < n; ++j) {
      printf("%d\t", array[i][j]);
    }
    printf("\n");
  }
  return 0;
}

