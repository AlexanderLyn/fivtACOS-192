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


#define kPortValue 7000


void start_connections(int descriptor, struct sockaddr* address, int len) {
  do {
    int new_desc = accept(descriptor, address, &len);
    printf("new connection %d\n", new_desc);
    if (new_desc == -1) {
      printf("accept error");
    }
    int buf_size = 100*sizeof(char);
    
    char *buffer = (char *)malloc(buf_size+sizeof(char));
    
    int recieve = 0;
    do {
      recieve = recv(new_desc, buffer, buf_size, 0);
      printf("recieved size = %d\t", recieve);
      buffer[recieve] = 0;
      printf("recieved msg = %s\n", buffer);
//      buffer[0] = 'A';
      char newdata[19] = "300/159/2_19/368/3";
      send(new_desc, newdata, strlen(newdata), 0);
    } while (recieve != 0);
    printf("closing\n");
    close(new_desc);
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
  if (error == -1) {
    printf("bind error");
  }
  int max_connections = 10;
  int err_listen = listen(sd, max_connections);
  int adresslen = sizeof(address);
  
  start_connections(sd, (struct sockaddr*) &address, adresslen);
  

  return 0;
}

