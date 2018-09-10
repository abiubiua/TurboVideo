//
//  tcpserver.c
//  TurboVideo
//
//  Created by luobiao on 2018/9/8.
//  Copyright © 2018年 luobiao. All rights reserved.
//

#include "tcpserver.h"
#include "log.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#define TCP_PORT 5200
#define MAX_CONN_LIMIT 50

typedef struct {
    int fd;
    int on;
    int port;
    char ip[128];
} fdstatus;

static int running = 0;

void StartTCPServer() {
    if (running) return;
    int err = 0;
    int on = 1;
    struct sockaddr_in localaddr;
    struct sockaddr_in clientaddr;
    socklen_t clientaddrlen = 0;
    fdstatus fds[MAX_CONN_LIMIT];
    int conncount = 0;
    int maxfd = 0;
    fd_set rset;
    struct timeval interval;
    LOGI("create socket");
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        LOGI("create listen fd failure");
    }
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));//enable reuse address
//    ioctl(listenfd, FIONBIO, &on, sizeof(on));//enable unblock
    
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(TCP_PORT);
    localaddr.sin_addr.s_addr = INADDR_ANY;
    LOGI("bind");
    if ((err = bind(listenfd, (struct sockaddr*)&localaddr, sizeof(localaddr))) < 0) {//bind
        LOGI("bind local socket err");
        return;
    }
    LOGI("listen");
    if ((err = listen(listenfd, 10)) < 0) {//listen
        LOGI("listen local socket err");
        return;
    }
    interval.tv_sec = 0;
    interval.tv_usec = 20 * 1000;
    running = 1;
    bzero(fds, sizeof(fds));
    while(running) {
        FD_ZERO(&rset);
        FD_SET(listenfd, &rset);//for listen socket
        if (listenfd > maxfd) {
            maxfd = listenfd;
        }
        if (conncount) {
            int cnt = 0;
            for (int i = 0; i < MAX_CONN_LIMIT; i++) {
                if (fds[i].on) {
                    ++cnt;
                    FD_SET(fds[i].fd, &rset);
                    if (fds[i].fd > maxfd) maxfd = fds[i].fd;
                }
                if (cnt == conncount) break;
            }
        }
        int n = select(maxfd + 1, &rset, NULL, NULL, &interval);
        if (n > 0) {
            if (FD_ISSET(listenfd, &rset)) {//check if listen socket ready
                --n;
                int clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientaddrlen);
                if (clientfd > 0) {
                    char buf[128] = {0};
                    struct sockaddr_in peeraddr;
                    socklen_t peeraddrlen = sizeof(peeraddr);
                    if (getpeername(clientfd, (struct sockaddr*)&peeraddr, &peeraddrlen) == 0) {
                        const char *ip = inet_ntop(peeraddr.sin_family, &peeraddr.sin_addr, buf, sizeof(buf));
                        int port = ntohs(peeraddr.sin_port);
                        printf("%s:%d connected\n", ip, port);
                    }
                    for (int i = 0; i < MAX_CONN_LIMIT; i++) {
                        if (!fds[i].on) {
                            fds[i].on = 1;
                            fds[i].fd = clientfd;
                            strncpy(fds[i].ip, buf, strlen(buf));
                            fds[i].port = ntohs(peeraddr.sin_port);
                            ++conncount;
                            break;
                        }
                    }
                } else {
                    LOGI("accept failure");
                    break;
                }
            }
            for (int i = 0; i < MAX_CONN_LIMIT; i++) {
                if (!fds[i].on) continue;
                if (FD_ISSET(fds[i].fd, &rset)) {//check client socket ready
                    char line[1024] = {0};
                    int nread = 0;
                    if ((nread = (int)read(fds[i].fd, line, sizeof(line))) <= 0) {
                        if (nread == 0) {
                            printf("EOF %s %d\n", fds[i].ip, fds[i].port);
                        } else {
                            printf("socket error %d errno %d %s %d\n", nread, errno, fds[i].ip, fds[i].port);
                        }
                        close(fds[i].fd);
                        bzero(&fds[i], sizeof(fdstatus));
                    } else {
                        printf("recv:%s\n", line);
                        char echo[1024] = {0};
                        snprintf(echo, sizeof(echo), "server echo:%s", line);
                        int nwrite = 0;
                        if ((nwrite = (int)write(fds[i].fd, echo, strlen(echo))) <= 0) {
                            printf("write error %d errno %d\n", nwrite, errno);
                        };
                    }
                    
                    --n;
                }
            }
        }
    }
}

void StopTCPServer() {
    if (!running) return;
    
}
