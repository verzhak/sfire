
#ifndef MEMORY_H
#define MEMORY_H

#include "all.h"
#include "exception.h"

void * sfire_alloc(unsigned type_size, unsigned dim, ...);
void sfire_free(void * mem);

#endif

