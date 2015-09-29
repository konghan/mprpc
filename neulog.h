/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#ifndef __NEULOG_H__
#define __NEULOG_H__

#ifdef __cplusplus
extern "C"{
#endif

#define NEULOG_MAX_BUF	    128

enum{
	NEULOG_UNKNOWN = 0,
	NEULOG_DEFAULT,
    NEULOG_TRACE,
    NEULOG_DEBUG,
    NEULOG_INFO,
    NEULOG_WARN,
    NEULOG_ERROR,
    NEULOG_FATAL
};


int neulog_print(int level, char *fmt, ...);

#define neulog_fatal(__fmt,...)	\
    neulog_print(NEULOG_FATAL,__fmt,##__VA_ARGS__)

#define neulog_error(__fmt,...)	\
    neulog_print(NEULOG_ERROR,__fmt,##__VA_ARGS__)

#define neulog_warn(__fmt,...)	\
    neulog_print(NEULOG_WARN,__fmt,##__VA_ARGS__)

#define neulog_info(__fmt,...)	\
    neulog_print(NEULOG_INFO,__fmt,##__VA_ARGS__)

#define neulog_debug(__fmt,...)	\
    neulog_print(NEULOG_DEBUG,__fmt,##__VA_ARGS__)

#define neulog_trace(__fmt,...)	\
    neulog_print(NEULOG_TRACE,__fmt,##__VA_ARGS__)

int neulog_init(const char *file, const char *addr, short port, int flags);
int neulog_fini();
void neulog_set(int level);

#ifdef __cplusplus
}
#endif

#endif // __NEULOG_H__


