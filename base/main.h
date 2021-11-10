#ifndef MAIN_H
#define MAIN_H

#include "Setting.h"

void* getBatch(void* con);

void sampling(INT *batch_h, INT *batch_t, INT *batch_r, REAL *batch_y, INT batchSize, INT negRate = 1, INT negRelRate = 0, INT headBatchFlag = 0);

#endif

