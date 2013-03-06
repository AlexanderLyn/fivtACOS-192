//
//  main.cpp
//  Second
//
//  Created by Alexey Goncharov on 22.02.13.
//  Copyright (c) 2013 DIHT. All rights reserved.
//

#include <iostream>
#include <vector>

int main(int argc, const char * argv[])
{
  int n = 100000000, i = 0, sum = 0;

  //  printf("enter n\n");
  
  // scanf("%d", &n);
  printf("vector started\n");
  time_t currentTime = 0;
  currentTime = time(NULL);
  std::vector<int> aArray(n), bArray(n), cArray(n);

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

