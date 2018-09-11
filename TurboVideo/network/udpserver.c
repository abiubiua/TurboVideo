//
//  udpserver.c
//  TurboVideo
//
//  Created by luobiao on 11/9/2018.
//  Copyright © 2018 luobiao. All rights reserved.
//

#include "udpserver.h"
#include "commoninclude.h"
#include "log.h"

static int running = 0;
static int UDP_PORT = 5200;
static const char* TAG = "udpserver";

void StartUDPServer()
{
    if (running) return;
    LOGD(TAG, "create UDP socket");
    int listensock = socket(AF_INET, SOCK_DGRAM, 0);
    if (listensock <= 0) return;
    struct sockaddr_in localaddr;
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(UDP_PORT);
    localaddr.sin_addr.s_addr = INADDR_ANY;
    
}

void StopUDPServer()
{
    
}
