//
//  main.c
//  utility_cat
//
//  Created by Alexey Goncharov on 31.03.13.
//  Copyright (c) 2013 DIHT. All rights reserved.
//

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char * argv[])
{
  char *nPath;
  if (argc > 1) {
    char *dirName = (char *)argv[1];
    int length = (int)strlen(dirName);
    nPath = malloc((2 + length)*sizeof(char));
    strcpy(nPath, "./");
    strcat(nPath, dirName);
  } else {
    printf("no file to print");
    return 0;
  }
  FILE *file = fopen(nPath, "r");
  if (file != NULL) {
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, file)) != -1) {
      printf("%s", line);
    }
  } else {
    printf("error while reading file");
  }
  return 0;
}

