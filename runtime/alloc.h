#ifndef SAPPHIRE_RUNTIME_ALLOC_H_
#define SAPPHIRE_RUNTIME_ALLOC_H_

#include <stdlib.h>

static void* alloc(long size) { return malloc(size); }

#endif
