/*
 * Copyright (c) 2013, Konghan. All rights reserved.
 * Distributed under the BSD license, see the LICENSE file.
 */

#ifndef __OSAPI_H__
#define __OSAPI_H__

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
#define ASSERT  assert
#else
#define ASSERT
#endif


//#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:	the pointer to the member.
 * @type:	the type of the container struct this is embedded in.
 * @member:	the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})

#define xmalloc    malloc
#define xfree      free
#define xalloc    calloc
#define xrealloc   realloc

#ifdef __cplusplus
}
#endif

#endif // __OSAPI_H__

