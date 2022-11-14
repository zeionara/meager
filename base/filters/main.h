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

bool doesMatchSomeFilterPatterns(struct FilterPatternsCollection filter_patterns, struct Triple triple);

#endif

