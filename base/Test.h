#ifndef TEST_H
#define TEST_H

#include "Setting.h"
#include "Reader.h"
#include "Corrupt.h"

void initTest();

void getHeadBatch(INT *ph, INT *pt, INT *pr);

void getValidHeadBatch(INT *ph, INT *pt, INT *pr);

void getTailBatch(INT *ph, INT *pt, INT *pr); 

void getValidTailBatch(INT *ph, INT *pt, INT *pr);

void testHead(REAL *probabilities, bool reverse = false);

void validHead(REAL *probabilities, bool reverse = false);

void testTail(REAL *probabilities, bool reverse = false);

void validTail(REAL *probabilities, bool reverse = false); 

void test_link_prediction(bool as_tsv = false); 

void getNegTest();

void getNegValid();

void getTestBatch(INT *ph, INT *pt, INT *pr, INT *nh, INT *nt, INT *nr);

void getValidBatch(INT *ph, INT *pt, INT *pr, INT *nh, INT *nt, INT *nr);

void getBestThreshold(REAL *relThresh, REAL *score_pos, REAL *score_neg);

void test_triple_classification(REAL *relThresh, REAL *score_pos, REAL *score_neg);

#endif

