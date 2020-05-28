#ifndef C_LOG_H
#define C_LOG_H

#include <stdio.h>
#include <stdarg.h>

typedef void (*CLogLockFn)(void *mutex, int lock);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define log_trace(...) c_clog_CLog(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) c_clog_CLog(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  c_clog_CLog(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  c_clog_CLog(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) c_clog_CLog(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) c_clog_CLog(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void c_clog_SetMutex(void *mutex);
void c_clog_SetLock(CLogLockFn fn);
void c_clog_SetFp(FILE *fp);
void c_clog_SetLevel(int level);
void c_clog_SetQuiet(int enable);

void c_clog_CLog(int level, const char *file, int line, const char *fmt, ...);

#endif
