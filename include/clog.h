#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>

typedef void (*log_LockFn)(void *mutex, int lock);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define log_trace(...) log_CLog(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_CLog(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_CLog(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_CLog(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_CLog(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_CLog(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void log_SetMutex(void *mutex);
void log_SetLock(log_LockFn fn);
void log_SetFp(FILE *fp);
void log_SetLevel(int level);
void log_SetQuiet(int enable);

void log_CLog(int level, const char *file, int line, const char *fmt, ...);

#endif
