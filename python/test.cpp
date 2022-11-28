#include "../base/Setting.h"
// #include "../base/Test.h"

//
//  Test
//

extern "C"
void get_head_batch(INT *ph, INT *pt, INT *pr) {
    // getHeadBatch(ph, pt, pr);
}


extern "C"
void get_tail_batch(INT *ph, INT *pt, INT *pr) {
    // getTailBatch(ph, pt, pr);
}

extern "C"
void test_tail(REAL *probabilities) {
    // testTail(probabilities);
}

extern "C"
void test_head(REAL *probabilities) {
    // testHead(probabilities);
}

//
//  Validate
//

extern "C"
void get_valid_head_batch(INT *ph, INT *pt, INT *pr) {
    // getValidHeadBatch(ph, pt, pr);
}

extern "C"
void get_valid_tail_batch(INT *ph, INT *pt, INT *pr) {
    // getValidTailBatch(ph, pt, pr);
}

extern "C"
void valid_head(REAL *probabilities) {
    // validHead(probabilities);
}

extern "C"
void valid_tail(REAL *probabilities) {
    // validTail(probabilities);
}

extern "C"
void test_link_prediction_(bool as_tsv = false) {
    // test_link_prediction(as_tsv);
}

extern "C"
void init_test() {
    // initTest();
}

//
// Triple classification
//

extern "C"
void get_test_batch(INT *ph, INT *pt, INT *pr, INT *nh, INT *nt, INT *nr) {
    // getTestBatch(ph, pt, pr, nh, nt, nr);
}

extern "C"
void get_valid_batch(INT *ph, INT *pt, INT *pr, INT *nh, INT *nt, INT *nr) {
    // getValidBatch(ph, pt, pr, nh, nt, nr);
}

extern "C"
void get_best_threshold(REAL *relThresh, REAL *score_pos, REAL *score_neg) {
    // getBestThreshold(relThresh, score_pos, score_neg);
}

extern "C"
void test_triple_classification_(REAL *relThresh, REAL *score_pos, REAL *score_neg) {
    // test_triple_classification(relThresh, score_pos, score_neg);
}

