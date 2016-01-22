/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */
#include "logger.h"

#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#ifdef ANDROID_NDK
#include <android/log.h>
#endif

typedef struct logger{
    int		    log_init;
    int		    log_sock;

    pthread_mutex_t lock;

    struct sockaddr_in log_serv;
}logger_t;

struct logger_ltos {
    int	    ll_level;
    char    *ll_string;
};

static struct logger_ltos __log_ltos[] = {
    {LOGGER_UNKNOWN, "UNKNOWN"},
    {LOGGER_DEFAULT, "DEFAULT"},
    {LOGGER_TRACE,  "TRACE"},
    {LOGGER_DEBUG,  "DEBUG"},
    {LOGGER_INFO,   " INFO"},
    {LOGGER_WARN,   " WORN"},
    {LOGGER_ERROR,  "ERROR"},
    {LOGGER_FATAL,  "FATAL"},
};

static logger_t	    __log_data = {};
static int __log_level = LOGGER_INFO;

void logger_set(int level){
	__log_level = level;
}

static char *log_ltos(int level){
    if((level > LOGGER_FATAL)||(level < 0)){
        return (__log_ltos[LOGGER_UNKNOWN]).ll_string;
    }

    return (__log_ltos[level]).ll_string;
}

int logger_print(int level, char *fmt, ...){
    logger_t	*log = &__log_data;
    char	buf[LOGGER_MAX_BUF];
    int		size = 0;
    va_list	args;

    if(level < __log_level){
    	return 0;
    }

    size = snprintf(buf, LOGGER_MAX_BUF, "%s:", log_ltos(level));

    va_start(args, fmt);
    size += vsnprintf(buf+size, LOGGER_MAX_BUF-size-1, fmt, args);
    va_end(args);

    if(!log->log_init){
        buf[size] = '\0';
#ifdef ANDROID_NDK
        __android_log_print(level, "SharePie",  "%s", buf);
#else
        printf("%s", buf);
#endif
    }else{
        send(log->log_sock, buf, size, 0);
    }

    return 0;
}

int logger_init(const char *file, const char *addr, short port, int flags){
    logger_t	*log = &__log_data;

    log->log_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(log->log_sock < 0){
    	return -1;
    }

    log->log_serv.sin_family = AF_INET;
#ifdef ANDROID_NDK
    log->log_serv.sin_addr.s_addr = inet_addr(addr);
#else
    log->log_serv.sin_addr.s_addr = inet_addr(addr);
#endif
    log->log_serv.sin_port = htons(port);

    if(connect(log->log_sock, (struct sockaddr *)&log->log_serv, sizeof(struct sockaddr_in)) < 0){
    	close(log->log_sock);
    	return -1;
    }

    pthread_mutex_init(&log->lock, NULL);

    log->log_init = 1;

    return 0;
}

int logger_fini(){
    logger_t	*log = &__log_data;

    if(log->log_init){
	    log->log_init = 0;
        pthread_mutex_destroy(&log->lock);
	    close(log->log_sock);
    }

    return 0;
}

