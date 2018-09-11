//
//  log.h
//  TurboVideo
//
//  Created by luobiao on 2018/9/8.
//  Copyright © 2018年 luobiao. All rights reserved.
//

#ifndef log_h
#define log_h

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/timeb.h>

typedef enum LOG_LEVEL {
    kLevelDebug,
    kLevelInfo,
    kLevelWarning,
    kLevelError,
} LOG_LEVEL;

static const char *logLevels[4] = {"DEBUG", "INFO", "WARNING", "ERROR"};



#define MAX_LOG_LEN 1024

#define LEVEL(l, level) \
do {\
if (l == kLevelDebug) {\
level = logLevels[0];\
} else if (l == kLevelInfo) {\
level = logLevels[1];\
} else if (l == kLevelWarning) {\
level = logLevels[2];\
} else if (l == kLevelError) {\
level = logLevels[3];\
}\
} while(0)\

#define LOG(tag, l, format, args...) \
do {\
char logline[MAX_LOG_LEN] = {0};\
struct  tm      *ptm;\
struct  timeb   stTimeb;\
ftime(&stTimeb);\
ptm = localtime(&stTimeb.time);\
int timelen = snprintf(logline, sizeof(logline), "%04d-%02d-%02d %02d:%02d:%02d.%03d",\
        ptm->tm_year + 1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, stTimeb.millitm);\
const char* level = NULL;\
LEVEL(l, level);\
snprintf(&logline[timelen], MAX_LOG_LEN - timelen, " [%s] [%s] ", tag, level);\
int remaining = sizeof(logline) - (int)strlen(logline);\
snprintf(&logline[strlen(logline)], remaining, format, ##args);\
printf("%s\n", logline);\
} while(0)\

#define LOGD(tag, format, args...) LOG(tag, kLevelDebug, format, ##args)
#define LOGI(tag, format, args...) LOG(tag, kLevelInfo, format, ##args)
#define LOGW(tag, format, args...) LOG(tag, kLevelWarning, format, ##args)
#define LOGE(tag, format, args...) LOG(tag, kLevelError, format, ##args)


#endif /* log_h */
