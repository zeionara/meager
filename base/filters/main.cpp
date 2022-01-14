#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <regex>

#include "../Reader.h"
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

