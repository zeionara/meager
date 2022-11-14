#ifndef MAIN_FILTERS_READER
#define MAIN_FILTERS_READER

#include <cstdlib>
#include <regex>
#include <vector>
#include <string>

#include "../triple/main.h"

#include "reader.h"

using namespace std;


struct FilterPatternsCollection {

	bool found;
    vector<regex> items;

    FilterPatternsCollection(string filename, bool verbose = false, bool drop_duplicates = true) {
        try {
            items = readFilterPatterns(filename, verbose, drop_duplicates); 
        } catch (invalid_argument& e) {
            found = false;
            return;
        }
        found = true;
        return;
    }
};

extern FilterPatternsCollection* exclusionFilterPatterns;
extern FilterPatternsCollection* inclusionFilterPatterns;

bool doesMatchSomeFilterPatterns(struct FilterPatternsCollection filter_patterns, struct Triple triple);

extern "C"
void importFilterPatterns(bool verbose, bool drop_duplicates, bool enable_filters = false);

bool isAcceptableTriple(INT h, INT r, INT t);

#endif

