//
//  main.c
//  TurboVideo
//
//  Created by luobiao on 2018/9/8.
//  Copyright © 2018年 luobiao. All rights reserved.
//

#include <stdio.h>
#include "tcpserver.h"
#include "udpserver.h"
int main(int argc, const char * argv[]) {
    // insert code here...
//    StartTCPServer();
    StartUDPServer();
    return 0;
}
