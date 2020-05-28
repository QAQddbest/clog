#include <stdio.h>
#include <string.h>
#include <time.h>

#include "clog.h"

static struct {
    void *mutex;
    CLogLockFn lock;
    FILE *fp;
    int level;
    int quiet;
} config;


static const char *g_levelNames[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

#ifdef C_CLOG_MUL_THREAD
#define Lock() _Lock()
#define Unlock() _Unlock()
#else
#define Lock(queue) ((void)0)
#define Unlock(queue) ((void)0)
#endif

static void _Lock(void) {
    if (config.lock) {
        config.lock(config.mutex, 1);
    }
}


static void _Unlock(void) {
    if (config.lock) {
        config.lock(config.mutex, 0);
    }
}


void c_clog_SetMutex(void *mutex) {
    config.mutex = mutex;
}


void c_clog_SetLock(CLogLockFn fn) {
    config.lock = fn;
}


void c_clog_SetFp(FILE *fp) {
    config.fp = fp;
}


void c_clog_SetLevel(int level) {
    config.level = level;
}


void c_clog_SetQuiet(int enable) {
    config.quiet = enable ? 1 : 0;
}


void c_clog_CLog(int level, const char *file, int line, const char *fmt, ...) {
    if (level < config.level) {
        return;
    }

    /* Acquire lock */
    Lock();

    /* Get current time */
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);

    /* Log to stderr */
    if (!config.quiet) {
        va_list args;
        char buf[16];
        buf[strftime(buf, sizeof(buf), "%H:%M:%S", lt)] = '\0';
        fprintf(stderr, "%s %-5s %s:%d: ", buf, g_levelNames[level], file, line);
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fprintf(stderr, "\n");
        fflush(stderr);
    }

    /* Log to file */
    if (config.fp) {
        va_list args;
        char buf[32];
        buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
        fprintf(config.fp, "%s %-5s %s:%d: ", buf, g_levelNames[level], file, line);
        va_start(args, fmt);
        vfprintf(config.fp, fmt, args);
        va_end(args);
        fprintf(config.fp, "\n");
        fflush(config.fp);
    }

    /* Release lock */
    Unlock();
}

#undef Lock
#undef Unlock
