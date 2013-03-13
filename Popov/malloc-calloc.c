#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, const char * argv[])
{
  int *a, *b, *c;
  int n = 0, i = 0, sum = 0;
  n = 100000000;
  time_t t = time(NULL); 
  a = (int *)malloc(n*sizeof(int));
  b = (int *)malloc(n*sizeof(int));
  c = (int *)malloc(n*sizeof(int));
  printf("malloc %ld\n", time(NULL) - t);
  for (i = 0; i < n; ++i) {
    a[i] = i;
    b[i] = i;
  }
  printf("malloc %ld\n", time(NULL) - t);
  for (i = 0; i < n; ++i) {
    c[i] = a[i] + b[i];
    sum += c[i];
  }
  printf("malloc %ld\n", time(NULL) - t);
  t = time(NULL);
  a = (int *)calloc(sizeof(int), n);
  b = (int *)calloc(sizeof(int), n);
  c = (int *)calloc(sizeof(int), n);
  printf("calloc %ld\n", time(NULL) - t);
  
  for (i = 0; i < n; ++i) {
    a[i] = i;
    b[i] = i;
  }
  printf("calloc %ld\n", time(NULL) - t);
  for (i = 0; i < n; ++i) {
    c[i] = a[i] + b[i];
    sum += c[i];
  }
  printf("calloc %ld\n", time(NULL) - t);
  
  return 0;
}

