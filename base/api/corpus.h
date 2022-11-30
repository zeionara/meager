#ifndef API_CORPUS_H
#define API_CORPUS_H

// #define INT long
// #define REAL float

#include <cstring>
#include <cstdio>
#include <string>

#include "../storage/LocalTsvCorpus.h"

extern
LocalTsvCorpus<long>* corpus;

extern
void initCorpus(char *path, bool enableFilters = false, bool verbose = false);

extern
long countEntities(); 

extern
long countRelations(); 

extern
long countTriples(); 

extern
long countTrainTriples(); 

extern
long countTestTriples(); 

extern
long countValidTriples(); 

extern
void importFilter(bool dropDuplicates = true, bool verbose = false);

extern
void importPattern(bool verbose = false);

extern
void importTrain(bool dropPatternDuplicates = true, bool verbose = false);

extern
void importTest(bool verbose = false);

extern
void importValid(bool verbose = false);

extern
void importTypes(bool verbose = false);

#endif
