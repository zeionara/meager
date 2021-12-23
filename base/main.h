#ifndef MAIN_H
#define MAIN_H

#include "Setting.h"

void* getBatch(void* con);
enum Pattern {
    none = 0,
    symmetric = 1
};

struct Parameter {
	INT id;
	INT *batch_h;
	INT *batch_t;
	INT *batch_r;
	REAL *batch_y;
	INT batchSize;
	INT negRate;
	INT negRelRate;
	INT headBatchFlag;
};

// void sampling(INT *batch_h, INT *batch_t, INT *batch_r, REAL *batch_y, INT batchSize, INT negRate = 1, INT negRelRate = 0, INT headBatchFlag = 0);
void sampling(INT *batch_h, INT *batch_t, INT *batch_r, REAL *batch_y, INT batchSize, INT negRate = 1, INT negRelRate = 0, INT headBatchFlag = 0, enum Pattern pattern = none);

#endif

