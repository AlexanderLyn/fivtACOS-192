//
//  main.c
//  server
//
//  Created by Alexey Goncharov on 06.05.13.
//  Copyright (c) 2013 DIHT. All rights reserved.
//

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <malloc/malloc.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define kPortValue 7000

pthread_t update_thread;
pthread_attr_t attr;
pthread_mutex_t* lock;
char** coord;
int* conn_desc;
int connections;
int sd;
pthread_t* listener;
pthread_mutex_t updateLock;


void* update_func(void* arg)
{
  char* output = (char*)malloc(sizeof(char)*10000);
  //sprintf(output, "%s",coord[0]);
  int i;
  for (i=0; i<connections; ++i) {
    pthread_mutex_lock(&lock[i]);
    if (coord[i]!=NULL) {
      if(strlen(output)) strcat(output, "_");
      strcat(output, coord[i]);
    }
    pthread_mutex_unlock(&lock[i]);
  }
  printf("will send: %s\n",output);
  for (i=0;i<connections; ++i) {
    pthread_mutex_lock(&lock[i]);
    if(coord[i]) send(conn_desc[i], output, strlen(output), 0);
    pthread_mutex_unlock(&lock[i]);
  }
  pthread_mutex_unlock(&updateLock);
  return NULL;
}

void* connection_listener(void* arg)
{
  int buf_size = 100*sizeof(char);
  int new_desc = conn_desc[*((int*)arg)];
  char *buffer = malloc(101*sizeof(char));
  int myID = *((int*)arg);
  
  int recieve = 0;
  do {
    recieve = (int)recv(new_desc, buffer,(size_t) buf_size, 0);
    if (recieve<0) {
      perror("recv");
    }
    if (recieve>150) {
      continue;
    }
    if(strcmp(buffer, "terminate")==0) {
      int i;
      for (i=0; i<connections; ++i) {
        shutdown(conn_desc[i], 2);
      }
      //shutdown(sd, 2);
      //exit(0);
    }
    printf("recieved size = %d\t", recieve);
    buffer[recieve] = 0;
    printf("recieved msg = %s\n", buffer);
    char result[200] = "";
    char* splitter = strtok(buffer, "_");
    if(splitter!=NULL)
    {
      strcat(result, splitter);
      strcat(result, "/");
      char* new_desc_c = malloc(sizeof(char)*10);
      sprintf(new_desc_c, "%d",new_desc);
      strcat(result, new_desc_c);
      splitter = strtok(NULL, "_");
      while (splitter!=NULL) {
        strcat(result, "_");
        strcat(result, splitter);
        strcat(result, "/");
        strcat(result, new_desc_c);
        splitter = strtok(NULL, "_");
      }
      pthread_mutex_lock(&lock[myID]);
      coord[myID] = result;
      pthread_mutex_unlock(&lock[myID]);
      pthread_mutex_lock(&updateLock);
      pthread_join(update_thread, NULL);
      pthread_create(&update_thread, &attr, update_func, NULL);
      
    }
  } while (recieve != 0);
  pthread_mutex_lock(&lock[myID]);
  coord[myID] = NULL;
  pthread_mutex_unlock(&lock[myID]);
  printf("closing\n");
  close(new_desc);
  return NULL;
}

void start_connections(int descriptor, struct sockaddr* address, int len) {
  connections = 0;
  conn_desc = malloc(sizeof(int)*10);
  coord = malloc(sizeof(char*)*10);
  listener = malloc(10*sizeof(pthread_t));
  lock = malloc(10*sizeof(pthread_mutex_t));
  pthread_mutex_init(&updateLock, NULL);
  do {
    int new_desc = accept(descriptor, address,(socklen_t*) &len);
    printf("new connection %d\n", new_desc);
    if (new_desc < 0) {
      perror("accept");
    }
    if (connections>=sizeof(conn_desc)) {
      conn_desc = realloc(conn_desc, sizeof(conn_desc)*2);
      listener = realloc(listener, 2*sizeof(listener));
      coord = realloc(coord, 2*sizeof(coord));
      lock = realloc(lock, 2*sizeof(lock));
    }
    coord[connections] = malloc(sizeof(char)*200);
    conn_desc[connections] = new_desc;
    strcat(coord[connections],"0/0/");
    char t[10];
    sprintf(t,"%d",new_desc);
    strcat(coord[connections],t);
    pthread_mutex_init(&lock[connections], NULL);
    int curr = connections;
    pthread_create(&listener[connections],&attr,connection_listener,&curr);
    connections++;
    
  } while (1);
 
}

int main(int argc, const char * argv[])
{
  sd = socket(AF_INET, SOCK_STREAM, 0);
  int val = 1;
  setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(kPortValue);
  
  int error = bind(sd, (struct sockaddr*) &address, sizeof(address));
  if (error < 0) {
    perror("bind");
  }
  int max_connections = 10;
  int err_listen = listen(sd, max_connections);
  if (err_listen<0) {
    perror("listen");
  }
  int adresslen = sizeof(address);
  
	pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  
  start_connections(sd, (struct sockaddr*) &address, adresslen);
  

  return 0;
}

