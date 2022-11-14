#ifndef NONE_PATTERN_READER
#define NONE_PATTERN_READER

#include <cstdlib>

#include "../main.h"

void separateNoneTriples(Triple* triples, INT nTriples, bool verbose = false, bool drop_duplicates = true, bool enable_filters = false);

extern
std::vector<PatternInstance>** noneTriplePatternInstances;

#endif

