#ifndef INVERSE_PATTERN_READER
#define INVERSE_PATTERN_READER

#include <cstdlib>

#include "../main.h"

void separateInverseTriples(Triple* triples, INT nTriples, bool verbose = false, bool drop_duplicates = true, bool enable_filters = false);

// extern
// std::vector<PatternInstance> symmetricTriples;

extern
std::vector<PatternInstance>** inverseTriplePatternInstances;

#endif

