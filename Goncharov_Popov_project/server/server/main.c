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
char** coord;
int* conn_desc;
int connections;


void* update_func(void* arg)
{
  int i;
  char output[1000] = "";
  strcat(output, coord[1]);
  for (i=2; i<connections; ++i) {
    strcat(output, "_");
    strcat(output, coord[i]);
  }
  send(conn_desc[0], output, sizeof(output), 0);
  for (i=1; i<connections; ++i) {
    int j;
    char output2[1000] = "";
    strcat(output2, coord[0]);
    for (j=0; j<connections; ++j) {
      if (i!=j) {
        strcat(output2, "_");
        strcat(output2, coord[j]);
      }
    }
    send(conn_desc[j], output2, sizeof(output2), 0);
  }
  return NULL;
}

void* connection_listener(void* arg)
{
  int buf_size = 100*sizeof(char);
  int new_desc = conn_desc[*((int*)arg)];
  char *buffer = malloc(101*sizeof(char));
  
  int recieve = 0;
  do {
    recieve = (int)recv(new_desc, buffer,(size_t) buf_size, 0);
    printf("recieved size = %d\t", recieve);
    buffer[recieve] = 0;
    printf("recieved msg = %s\n", buffer);
    //      buffer[0] = 'A';
    char result[100];
    char* splitter = strtok(buffer, "_");
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
    coord[*((int*)arg)] = result;
    //char newdata[19] = "300/159/2_19/368/3";
    //send(new_desc, newdata, strlen(newdata), 0);
    
    
    pthread_join(update_thread, NULL);
    pthread_create(&update_thread, &attr, update_func, NULL);
  } while (recieve != 0);
  printf("closing\n");
  close(new_desc);
  return NULL;
}

void start_connections(int descriptor, struct sockaddr* address, int len) {
  connections = 0;
  conn_desc = malloc(sizeof(int)*10);
  coord = malloc(sizeof(char*)*10);
  pthread_t* listener = malloc(10*sizeof(pthread_t));  
  do {
    int new_desc = accept(descriptor, address,(socklen_t*) &len);
    printf("new connection %d\n", new_desc);
    if (new_desc < 0) {
      perror("accept");
    }
    if (connections>=sizeof(conn_desc)) {
      conn_desc = realloc(conn_desc, sizeof(conn_desc)*2);
      listener = realloc(listener, 2*sizeof(pthread_t));
      coord = realloc(coord, 2*sizeof(char*));
    }
    conn_desc[connections] = new_desc;
    pthread_create(&listener[connections],&attr,connection_listener,&connections);
    connections++;
    
  } while (1);
 
}

int main(int argc, const char * argv[])
{
  int sd = socket(AF_INET, SOCK_STREAM, 0);
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

