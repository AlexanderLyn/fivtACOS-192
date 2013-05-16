//
//  main.m
//  server2
//
//  Created by Nikita Popov on 07.05.13.
//  Copyright (c) 2013 NIALSoft. All rights reserved.
//

#import <Foundation/Foundation.h>

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
  @autoreleasepool {
    NSString* output = [[NSString alloc] init];
    int i;
    for (i=0; i<connections; ++i) if (coord[i]!=NULL){
      output = [output stringByAppendingFormat:@"%s_",coord[i]];
    }
    NSRange rang = {[output length] - 1,1};
    output = [output stringByReplacingCharactersInRange:rang withString:@""];
    char* outend = [output UTF8String];
    printf("will send: %s\n",outend);
    for (i=0;i<connections; ++i) {
      send(conn_desc[i], outend, sizeof(char)*[output length], 0);
    }
  }
  return NULL;
}

void* connection_listener(void* arg)
{
  @autoreleasepool {
    int buf_size = 100*sizeof(char);
    int new_desc = conn_desc[*((int*)arg)];
    char *buffer = malloc(101*sizeof(char));
    
    int recieve = 0;
    do {
      recieve = (int)recv(new_desc, buffer,(size_t) buf_size, 0);
      if (recieve<0) {
        perror("recv");
      }
      if (recieve>150) {
        continue;
      }
      if (recieve==0) {
        //coord[*((int*)arg)]=NULL;
        //return NULL;
      }
      printf("recieved size = %d\t", recieve);
      buffer[recieve] = 0;
      printf("recieved msg = %s\n", buffer);
      NSString* test = [[NSString alloc] init];
      NSArray* arr = [@(buffer) componentsSeparatedByString:@"_"];
      for (NSString* temp in arr) {
        test = [test stringByAppendingFormat:@"%@/%d_",temp,new_desc];
      }
      test = [test stringByReplacingOccurrencesOfString:@"\r\n" withString:@""];
      NSRange rang = {[test length] - 1,1};
      test = [test stringByReplacingCharactersInRange:rang withString:@""];
      if(![test isEqualToString:@""]) coord[*((int*)arg)] = [test UTF8String];
      
      pthread_join(update_thread, NULL);
      pthread_create(&update_thread, &attr, update_func, NULL);
    } while (recieve != 0);
    printf("closing\n");
    close(new_desc);
  }
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
    int curr = connections;
    pthread_create(&listener[connections],&attr,connection_listener,&curr);
    connections++;
    
  } while (1);
  
}

int main(int argc, const char * argv[])
{
  @autoreleasepool {
    signal(SIGPIPE, SIG_IGN);
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
    
    }
  return 0;
}

