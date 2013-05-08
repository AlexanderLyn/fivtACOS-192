//
//  main.c
//  pthread
//
//  Created by Alexey Goncharov on 05.04.13.
//  Copyright (c) 2013 DIHT. All rights reserved.
//

#include <stdio.h>
#include <pthread.h>
#include <malloc/malloc.h>

pthread_mutex_t lock1, lock2;

void *sum (void *arg) {
  
  int step = ((int *)arg)[2];
  pthread_mutex_lock(&lock1);
  int threadnumber = ((int *)arg)[1];
  ((int *)arg)[1]++;
  pthread_mutex_unlock(&lock1);
  int summ = 0;
  int start = threadnumber*step+1;
  int finish = (threadnumber+1)*step;
  for (int i = start; i <= finish; ++i) {
    summ+=i;
  }
  
  pthread_mutex_lock(&lock2);
  ((int *)arg)[0]+= summ;
  pthread_mutex_unlock(&lock2);
  return NULL;
}


int main(int argc, const char * argv[]) {
  int n = 100;
  int step = 10;
  int thr = n/step;
  pthread_mutex_init(&lock1, NULL);
  pthread_mutex_init(&lock2, NULL);

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_t *threads = (pthread_t *)malloc(thr*sizeof(pthread_t));
  int array[3] = {0, 0, step}; // summ, number of thread, step length
  for (int i = 0; i < thr; ++i) {
    pthread_create(&threads[i], &attr, sum , array);
  }

  for (int i = 0; i < thr; ++i) {
    pthread_join(threads[i], NULL);
  }
  int summ = array[0];

  return 0;
}

