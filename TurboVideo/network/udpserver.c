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
    int error = 0;
    int listensock = socket(AF_INET, SOCK_DGRAM, 0);
    if (listensock <= 0) return;
    struct sockaddr_in localaddr;
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(UDP_PORT);
    localaddr.sin_addr.s_addr = INADDR_ANY;
    LOGD(TAG, "bind port %d", UDP_PORT);
    if ((error = bind(listensock, (struct sockaddr*)&localaddr, sizeof(localaddr))) != 0) {
        LOGE(TAG, "bind error %d errno %d", error, errno);
        return;
    }
    running = 1;
    struct sockaddr_in remoteaddr;
    socklen_t remoteaddrlen = sizeof(remoteaddr);
    fd_set rset;
    struct timeval interval;
    interval.tv_sec = 0;
    interval.tv_usec = 20 * 1000;
    while (running) {
        FD_ZERO(&rset);
        FD_SET(listensock, &rset);
        int n = select(listensock + 1, &rset, NULL, NULL, &interval);
        if (n == 0) continue;
        if (n < 0) {
            LOGE(TAG, "udp select error %d errno %d", n, errno);
            break;
        }
        if (FD_ISSET(listensock, &rset)) {
            char line[1024] = {0};
            int nrecv = (int)recvfrom(listensock, line, sizeof(line), 0, (struct sockaddr*)&remoteaddr, &remoteaddrlen);
            LOGI(TAG, "recv:%s len=%d", line, nrecv);
            snprintf(line, sizeof(line), "server echo=%d", atoi(line));
            sendto(listensock, line, strlen(line), 0, (struct sockaddr*)&remoteaddr, remoteaddrlen);
        }
    }
}

void StopUDPServer()
{
    
}
