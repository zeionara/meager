#ifndef MAIN_H
#define MAIN_H

#include "Setting.h"
#include "patterns/main.h"

#include <vector>

void* getBatch(void* con);

// template<unsigned int nPatternInstanceSets>
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
    // std::vector<PatternInstance> (*patternInstanceSets)[3];
    std::vector<PatternInstance>** patternInstanceSets; // [nPatternInstanceSets];
};

// void sampling(INT *batch_h, INT *batch_t, INT *batch_r, REAL *batch_y, INT batchSize, INT negRate = 1, INT negRelRate = 0, INT headBatchFlag = 0);
void sampling(INT *batch_h, INT *batch_t, INT *batch_r, REAL *batch_y, INT batchSize, INT negRate = 1, INT negRelRate = 0, INT headBatchFlag = 0, enum Pattern pattern = none, int nObservedTriplesPerPatternInstance = 0);

#endif

