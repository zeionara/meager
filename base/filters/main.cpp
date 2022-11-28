#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <regex>

// #include "../Reader.h"
#include "main.h"

using namespace std;

bool doesMatchSomeFilterPatterns(FilterPatternsCollection filter_patterns, Triple triple) {
    // if (!filter_patterns.found) {
    //     return false;
    // }

    for (auto &filter_pattern: filter_patterns.items) {
        if (regex_match(triple.as_filterable_string(), filter_pattern)) {
            return true;
        }
    }
    return false;
}

FilterPatternsCollection* exclusionFilterPatterns;
FilterPatternsCollection* inclusionFilterPatterns;

extern "C"
void importFilterPatterns(bool verbose, bool drop_duplicates, bool enable_filters) {
    if (enable_filters) {
        exclusionFilterPatterns = new FilterPatternsCollection("excluding", verbose, drop_duplicates); // readFilterPatterns("excluding");
        inclusionFilterPatterns = new FilterPatternsCollection("including", verbose, drop_duplicates); // readFilterPatterns("excluding");
    }
}

bool isAcceptableTriple(INT h, INT r, INT t) {
    Triple triple = Triple(h, r, t);
    
    return (
        (!inclusionFilterPatterns->found || doesMatchSomeFilterPatterns(*inclusionFilterPatterns, triple)) && 
        (!exclusionFilterPatterns->found || !doesMatchSomeFilterPatterns(*exclusionFilterPatterns, triple))
    );
}
