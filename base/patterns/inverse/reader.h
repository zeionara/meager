#ifndef INVERSE_PATTERN_READER
#define INVERSE_PATTERN_READER

#include <cstdlib>

#include "../main.h"

void separateInverseTriples(bool verbose = false, bool drop_duplicates = true);

// extern
// std::vector<PatternInstance> symmetricTriples;

extern
std::vector<PatternInstance>** inverseTriplePatternInstances;

#endif

