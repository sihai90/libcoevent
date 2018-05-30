
#include "coevent.h"
#include "coevent_itnl.h"
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>
#include <fcntl.h>

using namespace andrewmc::libcoevent;

// ==========
#define __CO_EVENT_ITNL
#ifdef __CO_EVENT_ITNL

// reference: https://github.com/Andrew-M-C/Linux-Linux_Programming_Template/blob/master/AMCCommonLib.c
ssize_t andrewmc::libcoevent::print(int fd, const char *format, ...)
{
    char buff[CFG_PRINT_BUFF_SIZE];
    va_list vaList;
    size_t dateLen = 0;

    tzset();
    time_t currSec = time(0);
    struct tm currTime;
    struct tm *pTime = localtime(&currSec);
    struct timeval currDayTime;

    gettimeofday(&currDayTime, NULL);
    if (pTime)
    {
        memcpy(&currTime, pTime, sizeof(currTime));
        dateLen = sprintf(buff, "%04d-%02d-%02d,%02d:%02d:%02d.%06ld ", 
                            currTime.tm_year + 1900, currTime.tm_mon + 1, currTime.tm_mday,
                            currTime.tm_hour, currTime.tm_min, currTime.tm_sec, currDayTime.tv_usec);
    }

    va_start(vaList, format);
    vsnprintf((char *)(buff + dateLen), sizeof(buff) - dateLen - 1, format, vaList);
    va_end(vaList);

    dateLen = strlen(buff);
    buff[dateLen + 0] = '\n';
    buff[dateLen + 1] = '\0';

    return (write(fd, buff, dateLen + 1));
}


struct timeval andrewmc::libcoevent::to_timeval(double seconds)
{
    struct timeval ret;
    if (seconds > 0) {
        ret.tv_sec = (time_t)seconds;
        ret.tv_usec = (suseconds_t)((seconds - ret.tv_sec) * 1000000);
    }
    return ret;
}


struct timeval andrewmc::libcoevent::to_timeval(float seconds)
{
    struct timeval ret;
    if (seconds > 0) {
        ret.tv_sec = (time_t)seconds;
        ret.tv_usec = (suseconds_t)((seconds - ret.tv_sec) * 1000000);
    }
    return ret;
}


struct timeval andrewmc::libcoevent::to_timeval(int seconds)
{
    struct timeval ret;
    if (seconds > 0) {
        ret.tv_sec = (time_t)seconds;
        ret.tv_usec = 0;
    }
    return ret;
}


struct timeval andrewmc::libcoevent::to_timeval(long seconds)
{
    struct timeval ret;
    if (seconds > 0) {
        ret.tv_sec = (time_t)seconds;
        ret.tv_usec = 0;
    }
    return ret;
}


double andrewmc::libcoevent::to_double(struct timeval &time)
{
    double ret;
    ret = (double)(time.tv_sec);
    ret += ((double)(time.tv_usec)) / 1000000.0;
    return ret;
}


BOOL andrewmc::libcoevent::is_coroutine_end(const struct stCoRoutine_t *routine)
{
    if (routine) {
        return (routine->cEnd) ? TRUE : FALSE;
    }
    return TRUE;
}


BOOL andrewmc::libcoevent::is_coroutine_started(const struct stCoRoutine_t *routine)
{
    if (routine) {
        return (routine->cStart) ? TRUE : FALSE;
    }
    return FALSE;
}


int andrewmc::libcoevent::set_fd_nonblock(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    if (flag >= 0)
    {
        return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
    }
    else
    {
        return flag;
    }
}

#endif  // end of libcoevent::print


