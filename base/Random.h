#ifndef RANDOM_H
#define RANDOM_H

#include "types.h"
#include <cstdlib>

extern "C"
void randReset(INT nWorkers);

unsigned long long randd(INT id);

INT rand_max(INT id, INT x); 

INT rand(INT a, INT b);

#endif

