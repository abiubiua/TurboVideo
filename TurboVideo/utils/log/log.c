//
//  log.c
//  TurboVideo
//
//  Created by luobiao on 11/9/2018.
//  Copyright © 2018 luobiao. All rights reserved.
//

#include <stdio.h>
#include "log.h"


//void  LOG(const char* tag, int l, const char* format, args...)
//{
//    do {
//        char line[MAX_LOG_LEN] = {0};
//        time_t rawtime;
//        struct tm * timeinfo;
//        time(&rawtime);
//        timeinfo = localtime(&rawtime);
//        int timelen = (int)strftime(line, sizeof(line), "%F %X", timeinfo);
//        const char* level = NULL;
//        LEVEL(l, level);
//        snprintf(&line[timelen], MAX_LOG_LEN - timelen, " [%s] [%s] ", tag, level);
//        int remaining = sizeof(line) - (int)strlen(line);
//        snprintf(&line[strlen(line)], remaining, format, args);
//        printf(line);
//    } while(0);
//}

