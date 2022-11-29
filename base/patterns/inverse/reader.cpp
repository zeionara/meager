#include <fstream>
#include <vector>
#include <unordered_set>

// #include "main.h"
// #include "../../Reader.h"
#include "../../storage/CorpusReader.h"
#include "../BinaryPatternRelationMap.h"
#include "reader.h"

using namespace std;

// const int nTriplesPerPattern = 2;

// vector<PatternInstance> symmetricTriples;
vector<PatternInstance>** inverseTriplePatternInstances = (vector<PatternInstance>**)malloc(sizeof(vector<PatternInstance>*) * (nTriplesPerInversePattern + 1));
