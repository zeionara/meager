#ifndef INVERSE_PATTERN_READER
#define INVERSE_PATTERN_READER

#include <cstdlib>

#include "../main.h"

#include "../../triple/TripleIndex.h"

void separateInverseTriples(Triple* triples, INT nTriples, TripleIndex* index, bool verbose = false, bool drop_duplicates = true, bool enable_filters = false);

// extern
// std::vector<PatternInstance> symmetricTriples;

extern
std::vector<PatternInstance>** inverseTriplePatternInstances;

#endif

