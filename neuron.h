

#ifndef __NEURON_H__
#define __NEURON_H__

#include "neu-worker.h"
#include "neu-service.h"
#include "neu-session.h"

#include "neu-handle.h"
#include "neulog.h"

#include <assert.h>

#ifndef DEBUG
#define ASSERT(exp)
#else
#define ASSERT(exp)    assert(exp)
#endif 

#ifndef XMALLOC
#define xmalloc     malloc
#define xfree       free
#endif

enum {
    NEUERROR_PDUBROKEN = 1,
    NEUERROR_MSGERROR,
};


#endif // __NEURON_H__

