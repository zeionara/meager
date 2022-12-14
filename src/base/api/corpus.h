#ifndef API_CORPUS_H
#define API_CORPUS_H

// #define INT long
// #define REAL float

#include <cstring>
#include <cstdio>
#include <string>

#include "../corpus/local/Default.h"

extern
corpus::local::Default<long>* corpus;

extern
void initCorpus(char *path, bool enableFilters = false, bool verbose = false);

extern
long countEntities(bool verbose = false); 

extern
long countRelations(bool verbose = false); 

extern
long countTriples(SubsetType subset, bool verbose = false);

extern
long countTriples(bool verbose = false); 

// extern
// long countTrainTriples(); 
// 
// extern
// long countTestTriples(); 
// 
// extern
// long countValidTriples(); 

extern
void importFilter(bool dropDuplicates = true, bool verbose = false);

extern
void importPattern(bool verbose = false);

extern
void importTrain(bool dropPatternDuplicates = true, bool verbose = false);

extern
void importTriples(SubsetType subset, bool verbose = false);

// extern
// void importTest(bool verbose = false);
// 
// extern
// void importValid(bool verbose = false);

extern
void importTypes(bool verbose = false);

#endif
