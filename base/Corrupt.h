#ifndef CORRUPT_H
#define CORRUPT_H

#include "Random.h"
#include "triple/main.h"
#include "Reader.h"

INT corrupt_head(INT id, INT h, INT r);

INT corrupt_tail(INT id, INT t, INT r);

INT corrupt_rel(INT id, INT h, INT t);

bool _find(INT h, INT t, INT r);

INT corrupt(INT h, INT r);

#endif

