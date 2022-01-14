#ifndef READER_H
#define READER_H

#include "Setting.h"
#include "Triple.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>

#include "TripleIndex.h"

extern
INT *freqRel, *freqEnt;
extern
INT *lefHead, *rigHead;
extern
INT *lefTail, *rigTail;
extern
INT *lefRel, *rigRel;
extern
REAL *left_mean, *right_mean;

extern
Triple *trainList, *trainHead, *trainTail, *trainRel;

extern
INT *testLef, *testRig;
extern
INT *validLef, *validRig;

extern
Triple *testList, *validList, *tripleList;

extern
INT *head_lef;
extern
INT *head_rig;
extern
INT *tail_lef;
extern
INT *tail_rig;
extern
INT *head_type;
extern
INT *tail_type;

void print_triples(std::string header, Triple* triples, int nTriples);

extern "C"
void importFilterPatterns(bool verbose = false, bool drop_duplicates = true);

extern "C"
void importTrainFiles(bool verbose = false); 

extern "C"
void importTestFiles(bool verbose = false);

extern "C"
void importTypeFiles();

// Pattern-specific constructs

extern
TripleIndex* trainTripleIndex;

#endif

