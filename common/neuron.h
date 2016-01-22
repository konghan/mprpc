

#ifndef __NEURON_H__
#define __NEURON_H__

#include <assert.h>
#include <stdlib.h>

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
    NEUERROR_PDUERR = 1,
    NEUERROR_MSGERR,
};


#endif // __NEURON_H__

