//
//  ViewController.m
//  Points_client
//
//  Created by Alexey Goncharov on 07.05.13.
//  Copyright (c) 2013 DIHT. All rights reserved.
//

#import "ViewController.h"

#define kPortValue 7000
#define kServerIP "192.168.200.8"
#define kPointWidth 60

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  self.view.multipleTouchEnabled = YES;
  
  UITextField *textField = [[UITextField alloc]initWithFrame:CGRectMake(10, 20, 150, 30)];
  [textField setBorderStyle:UITextBorderStyleRoundedRect];
  textField.delegate = self;
  textField.text = @"192.168.200.4";
  textField.keyboardType = UIKeyboardTypeNumbersAndPunctuation;
  textField.returnKeyType = UIReturnKeyJoin;
  [self.view addSubview:textField];
  [textField becomeFirstResponder];
  ipField = textField;
  
  connectButton = [UIButton buttonWithType:UIButtonTypeRoundedRect];
  connectButton.frame = CGRectMake(textField.frame.origin.x + textField.frame.size.width + 10, 20, 100, 40);
  [connectButton setTitle:@"Connect" forState:UIControlStateNormal];
  [connectButton addTarget:self action:@selector(connectButtonPressed:) forControlEvents:UIControlEventTouchUpInside];
  [self.view addSubview:connectButton];
  
  _touches = [NSMutableArray arrayWithCapacity:10];
  buf_size = 100*sizeof(char);
  data_buffer = (char *)malloc(buf_size+sizeof(char));
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField {
  server_ip = (char *)[textField.text UTF8String];
  [textField resignFirstResponder];
  [self performSelectorInBackground:@selector(loadServerConnection) withObject:nil];
  return YES;
}

- (void)connectButtonPressed:(UIButton *)button {
  [ipField resignFirstResponder];
  server_ip = (char *)[ipField.text UTF8String];
  if (socket_descriptor > 0) {
    [self closeServerConnection];
  } else {
    [self performSelectorInBackground:@selector(loadServerConnection) withObject:nil];
  }
}
 
- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
}

#pragma mark- touches handling

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
  for (UITouch *touch in touches) {
    CGPoint coordinate = [touch locationInView:self.view];
//    UIView *newView = [self getPointForCoordinate:coordinate];
    [_touches addObject:touch];
  }
  [self sendTouchesOnServer];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
  [self sendTouchesOnServer];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
  for (UITouch *touch in touches) {
    [_touches removeObject:touch];
  }
  [self sendTouchesOnServer];
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
  for (UITouch *touch in touches) {
    [_touches removeObject:touch];
  }
  [self sendTouchesOnServer];
}

- (UIView *)getPointForCoordinate:(CGPoint)center {
  UIView *newView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, kPointWidth, kPointWidth)];
  newView.center = center;
  newView.backgroundColor = [UIColor redColor];
  newView.tag = 1;
  return newView;
}

- (void)updateTouchesFromServer:(NSArray *)touches {
  for (UIView *viewToDelete in self.view.subviews) {
    if (viewToDelete.tag == 2) {
      [viewToDelete removeFromSuperview];
    }
  }
  for (NSDictionary *touch in touches) {
    NSValue *point = touch[@"Point"];
    UIColor *color = touch[@"Color"];
    UIView *newPoint = [self getPointForCoordinate:[point CGPointValue]];
    newPoint.backgroundColor = color;
    newPoint.tag = 2;
    [self.view addSubview:newPoint];
  }
}

- (void)sendTouchesOnServer {
  NSString *data = [[NSString alloc]init];
  for (UITouch *touchToSend in _touches) {
//    if (viewToSend.tag != 1) {
//      continue;
//    }
    CGPoint center = [touchToSend locationInView:self.view];
    if (data.length > 0) {
      data = [data stringByAppendingFormat:@"_"];
    }
    data = [data stringByAppendingFormat:@"%d/%d", (int)center.x, (int)center.y];
  }
  [self sendDataOnServer:[data UTF8String]];

}

#pragma mark- server functions

- (int)loadServerConnection {
  socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  he = gethostbyname(server_ip);
  
  server.sin_family = AF_INET;
  bcopy(he->h_addr_list[0], &server.sin_addr.s_addr, he->h_length);
  server.sin_port = htons(kPortValue);
  int connect_error = connect(socket_descriptor, (struct sockaddr*) &server, sizeof(server));
  if (connect_error < 0) {
    perror("connect");
    socket_descriptor = -1;
    return -1;
  }
  printf("connection established\n");
  
  [connectButton setTitle:@"Disonnect" forState:UIControlStateNormal];

  [self performSelectorInBackground:@selector(waitDataFromServer) withObject:nil];
  return 0;
}

- (void)closeServerConnection {
  close(socket_descriptor);
  socket_descriptor = -1;
  [connectButton setTitle:@"Connect" forState:UIControlStateNormal];
}

- (void)sendDataOnServer:(const char *)data {
  if (socket_descriptor <= 0) {
    return;
  }
  send(socket_descriptor, data, strlen(data), 0);
}

- (void)waitDataFromServer {
  while (socket_descriptor>0) {
    data_buffer = (char *)malloc(200*sizeof(char));
    int recieve = 0;
    recieve = recv(socket_descriptor, data_buffer, sizeof(char)*200, 0);
    if (!data_buffer) return;
    data_buffer[recieve] = 0;
    printf("recieved msg = %s\n", data_buffer);
    [self parceRecievedData:data_buffer];
  }
}

#pragma mark- recieved data parcing

- (void)parceRecievedData:(char *)data {
  if (data[0] == 0) {
    return;
  }
  NSMutableArray *newTouches = [NSMutableArray array];
  NSString *recievedData = [NSString stringWithUTF8String:data];
  NSArray *array = [recievedData componentsSeparatedByString:@"_"];
  for (NSString *currentTouch in array) {
    NSArray *components = [currentTouch componentsSeparatedByString:@"/"];
    if (components.count != 3) {
      return;
    }
    UIColor *color = [self colorWithInt:[components[2] intValue]];
    int x = [components[0] intValue];
    int y = [components[1] intValue];
    NSDictionary *curTouch = @{@"Point": [NSValue valueWithCGPoint:CGPointMake(x, y)], @"Color" : color};
    [newTouches addObject:curTouch];
  }
  [self performSelectorOnMainThread:@selector(updateTouchesFromServer:) withObject:newTouches waitUntilDone:NO];
}

- (UIColor *)colorWithInt:(int)numer {
  if (numer %5 == 0) {
    return [UIColor grayColor];
  }
  if (numer %5 == 1) {
    return [UIColor greenColor];
  }
  if (numer %5 == 2) {
    return [UIColor blackColor];
  }
  if (numer %5 == 3) {
    return [UIColor yellowColor];
  }
  if (numer %5 == 4) {
    return [UIColor blueColor];
  }
  return [UIColor redColor];
}

@end
