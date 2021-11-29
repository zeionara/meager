#include "../base/Setting.h"
#include "../base/main.h"
// #include <iostream>

extern "C"
void sample(INT *batch_h, INT *batch_t, INT *batch_r, REAL *batch_y, INT batchSize, INT negRate = 1, INT negRelRate = 0, INT headBatchFlag = 0) {
    // std::cout << "Trying to start sampling";
    return sampling(batch_h, batch_t, batch_r, batch_y, batchSize, negRate, negRelRate, headBatchFlag);
}

