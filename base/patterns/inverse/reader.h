#ifndef INVERSE_PATTERN_READER
#define INVERSE_PATTERN_READER

#include <cstdlib>

#include "../main.h"

#include "../../triple/TripleIndex.h"
#include "../../storage/CorpusReader.h"

// template <typename T>
// void separateInverseTriples(string path, Triple* triples, INT nTriples, TripleIndex* index, CorpusReader<T>* reader, bool verbose = false, bool drop_duplicates = true, bool enable_filters = false);
// template <typename T>
void separateInverseTriples(string path, Triple* triples, INT nTriples, TripleIndex* index, CorpusReader<INT>* reader, bool verbose, bool drop_duplicates, bool enable_filters);

// extern
// std::vector<PatternInstance> symmetricTriples;

extern
std::vector<PatternInstance>** inverseTriplePatternInstances;

#endif

