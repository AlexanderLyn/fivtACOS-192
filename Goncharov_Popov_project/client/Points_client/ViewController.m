//
//  ViewController.m
//  Points_client
//
//  Created by Alexey Goncharov on 07.05.13.
//  Copyright (c) 2013 DIHT. All rights reserved.
//

#import "ViewController.h"

#define kPortValue 7000

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
  [super viewDidLoad];
  self.view.multipleTouchEnabled = YES;
  _touches = [NSMutableArray arrayWithCapacity:10];
  _views = [NSMutableArray arrayWithCapacity:10];
  buf_size = 100*sizeof(char);
  data_buffer = (char *)malloc(buf_size+sizeof(char));
  [self loadServerConnection];
}
 
- (void)didReceiveMemoryWarning {
  [super didReceiveMemoryWarning];
}

#pragma mark- touches handling

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
//  NSLog(@"touch began: ");
  for (UITouch *touch in touches) {
    CGPoint coordinate = [touch locationInView:self.view];
    UIView *newView = [self getPointForCoordinate:coordinate];
    [_views addObject:newView];
    [_touches addObject:touch];
    [self.view addSubview:newView];
  }
  [self sendTouchesOnServer];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
//  NSLog(@"touch moved: ");
  for (UITouch *touch in touches) {
    CGPoint coordinate = [touch locationInView:self.view];
    int index = [_touches indexOfObject:touch];
    UIView *currentView = _views[index];
    currentView.center = coordinate;
  }
  [self sendTouchesOnServer];
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {
//  NSLog(@"touch ended: ");
  for (UITouch *touch in touches) {
    int index = [_touches indexOfObject:touch];
    UIView *currentView = _views[index];
    [currentView removeFromSuperview];
    [_views removeObject:currentView];
    [_touches removeObject:touch];
  }
  [self sendTouchesOnServer];
}

- (UIView *)getPointForCoordinate:(CGPoint)center {
  UIView *newView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, 40, 40)];
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
  for (UIView *viewToSend in self.view.subviews) {
    if (viewToSend.tag != 1) {
      continue;
    }
    CGPoint center = viewToSend.center;
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
  he = gethostbyname("localhost");
  
  server.sin_family = AF_INET;
  bcopy(he->h_addr_list[0], &server.sin_addr.s_addr, he->h_length);
  server.sin_port = htons(kPortValue);
  
  int connect_error = connect(socket_descriptor, (struct sockaddr*) &server, sizeof(server));
  if (connect_error == -1) {
    perror("connect");
    printf("connection failed");
    return -1;
  } else {
    printf("connected successfully");
    [self performSelectorInBackground:@selector(waitDataFromServer) withObject:nil];
    return 0;
  }
}

- (void)closeServerConnection {
  close(socket_descriptor);
}

- (void)sendDataOnServer:(const char *)data {
//  NSLog(@"sending now: %s", data);
  int data_length = strlen(data);
  for (int i = 0; i < data_length; ++i) {
    data_buffer[i] = data[i];
  }
  data_buffer[data_length] = 0;
  send(socket_descriptor, data_buffer, data_length, 0);
}

- (void)waitDataFromServer {
  while (1) {
    int recieve = recv(socket_descriptor, data_buffer, buf_size, 0);
    if (!data_buffer) return;
    data_buffer[recieve] = 0;
    //  printf("recieved msg = %s\n", data_buffer);
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
    if (components.count < 3) {
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
  return [UIColor greenColor];
}

@end
