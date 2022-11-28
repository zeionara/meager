#ifndef SYMMETRIC_PATTERN_READER
#define SYMMETRIC_PATTERN_READER

#include <cstdlib>

#include "../main.h"
#include "../../triple/TripleIndex.h"

void separateSymmetricTriples(string path, Triple* triples, INT nTriples, TripleIndex* index, bool verbose = false, bool drop_duplicates = true);

// extern
// std::vector<PatternInstance> symmetricTriples;

extern
std::vector<PatternInstance>** symmetricTriplePatternInstances;

#endif

