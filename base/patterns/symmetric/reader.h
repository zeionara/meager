#ifndef SYMMETRIC_PATTERN_READER
#define SYMMETRIC_PATTERN_READER

#include <cstdlib>

#include "../main.h"

void separateSymmetricTriples(bool verbose = false, bool drop_duplicates = true);

extern
std::vector<PatternInstance> symmetricTriples;

extern
std::vector<PatternInstance>** symmetricTriplePatternInstances;
// extern
// int nSymmetricTriples;

#endif

