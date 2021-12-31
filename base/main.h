#ifndef MAIN_H
#define MAIN_H

#include <vector>

#include "Setting.h"
#include "patterns/main.h"
#include "patterns/none/main.h"

// void* getBatch(void* con);

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
    int nObservedTriplesPerPatternInstance;
    std::vector<PatternInstance>** patternInstanceSets;
};

void sampling(INT *batch_h, INT *batch_t, INT *batch_r, REAL *batch_y, INT batchSize, INT negRate = 1, INT negRelRate = 0, INT headBatchFlag = 0, string pattern = nonePatternName, int nObservedTriplesPerPatternInstance = 0);

#endif

