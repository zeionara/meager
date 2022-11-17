#ifndef READER_H
#define READER_H

#include "Setting.h"
#include "triple/main.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_map>
#include "triple/list/main.h"
#include "triple/type.h"

#include "triple/TripleIndex.h"

extern
ThickTripleListWrapper* trainList;
extern
ThinTripleListWrapper* testList, *validList;
extern
RelationTypes* types;

void print_triples(std::string header, Triple* triples, int nTriples);

extern "C"
void importTrainFiles(bool verbose = false, bool enable_filters = false); 

extern "C"
void importTestFiles(bool verbose = false, bool enable_filters = false);

extern "C"
void importTypeFiles();

#endif

