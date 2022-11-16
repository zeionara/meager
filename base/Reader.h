#ifndef READER_H
#define READER_H

#include "Setting.h"
#include "triple/main.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "triple/list.h"
#include "triple/type.h"

#include "triple/TripleIndex.h"

extern
TrainTripleLists* trainLists;
extern
TestTripleLists* testLists, *validLists;
extern
RelationTypes* types;
// extern
// INT *freqRel, *freqEnt;
// extern
// INT *lefHead, *rigHead;
// extern
// INT *lefTail, *rigTail;
// extern
// INT *lefRel, *rigRel;
// extern
// REAL *left_mean, *right_mean;
// 
// extern
// Triple *trainList, *trainHead, *trainTail, *trainRel;
// 
// extern
// INT *testLef, *testRig;
// extern
// INT *validLef, *validRig;
// 
// extern
// Triple *testList, *validList, *tripleList;
// 
// extern
// INT *head_lef;
// extern
// INT *head_rig;
// extern
// INT *tail_lef;
// extern
// INT *tail_rig;
// extern
// INT *head_type;
// extern
// INT *tail_type;

void print_triples(std::string header, Triple* triples, int nTriples);

// extern "C"
// void importFilterPatterns(bool verbose = false, bool drop_duplicates = true, bool enable_filters = false);

extern "C"
void importTrainFiles(bool verbose = false, bool enable_filters = false); 

extern "C"
void importTestFiles(bool verbose = false, bool enable_filters = false);

extern "C"
void importTypeFiles();

// Pattern-specific constructs

// extern
// TripleIndex* trainTripleIndex;
// 
// extern
// std::vector<INT> internal_to_external_entity_id;
// extern
// std::vector<INT> internal_to_external_relation_id;
// 
// extern
// std::unordered_map<INT, INT> external_to_internal_entity_id;
// extern
// std::unordered_map<INT, INT> external_to_internal_relation_id;
#endif

