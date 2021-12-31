#ifndef NONE_PATTERN_READER
#define NONE_PATTERN_READER

#include <cstdlib>

#include "../main.h"

void separateNoneTriples(bool verbose = false, bool drop_duplicates = true);

extern
std::vector<PatternInstance>** noneTriplePatternInstances;

#endif

