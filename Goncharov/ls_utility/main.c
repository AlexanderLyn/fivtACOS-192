//
//  main.c
//  utility_ls
//
//  Created by Alexey Goncharov on 31.03.13.
//  Copyright (c) 2013 DIHT. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glob.h>
#include <dirent.h>


int main(int argc, const char * argv[])
{

  DIR *dpdf;
  struct dirent *epdf;
  int i = 0;
  char path[2] = "./";
  char *nPath;
  if (argc > 1) {
    char *dirName = argv[1];
    int length = (int)strlen(dirName);
    nPath = malloc(sizeof(char)*(2+length));
    nPath[0] = '.';
    nPath[1] = '/';
    for (int i = 2; i < 2 + length; ++i) {
      nPath[i] = dirName[i-2];
    }
  } else {
    nPath = malloc(sizeof(char)*(2));
    nPath[0] = '.';
    nPath[1] = '/';
  }
//  printf("%s", nPath);
  if (argc > 1) {
    dpdf = opendir(nPath);
  } else {
    dpdf = opendir(path);
  }
  int hidden = 0;
  if (dpdf != NULL){
    while ((epdf = readdir(dpdf))){
      char *nameOfFile = epdf->d_name;
      if (nameOfFile[0] != '.' || hidden == 1) {
        printf("%s\n", nameOfFile);
        ++i;
      }
    }
  }
  if (i == 0) {
    printf("no files in such directory or directory doesn't exists");
  }
  return 0;
}

