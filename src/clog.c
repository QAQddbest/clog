#include <stdio.h>
#include <string.h>
#include <time.h>

#include "clog.h"

static struct {
    void *mutex;
    log_LockFn lock;
    FILE *fp;
    int level;
    int quiet;
} config;


static const char *g_levelNames[] = {
        "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};


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


void log_SetMutex(void *mutex) {
    config.mutex = mutex;
}


void log_SetLock(log_LockFn fn) {
    config.lock = fn;
}


void log_SetFp(FILE *fp) {
    config.fp = fp;
}


void log_SetLevel(int level) {
    config.level = level;
}


void log_SetQuiet(int enable) {
    config.quiet = enable ? 1 : 0;
}


void log_CLog(int level, const char *file, int line, const char *fmt, ...) {
    if (level < config.level) {
        return;
    }

    /* Acquire lock */
    _Lock();

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
    _Unlock();
}