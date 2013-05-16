//
//  ViewController.h
//  Points_client
//
//  Created by Alexey Goncharov on 07.05.13.
//  Copyright (c) 2013 DIHT. All rights reserved.
//

#import <UIKit/UIKit.h>

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <malloc/malloc.h>
#include <netdb.h>
#include <string.h>

@interface ViewController : UIViewController <UITextFieldDelegate> {
  NSMutableArray *_touches;
  UITextField *ipField;
  UIButton *connectButton;
  
  //server values
  int socket_descriptor;
  struct hostent *he;
  struct sockaddr_in server;
  char *data_buffer;
  int buf_size;
  char *server_ip;
}

@end

