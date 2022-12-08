#ifndef FILTERS_FILTER_PATTERNS_H
#define FILTERS_FILTER_PATTERNS_H

#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <unordered_set>

#include "../triple/main.h"
#include "../storage/reader/CorpusReader.h"

using namespace std;
using namespace meager::main::storage;

template <typename T>
struct FilterPatterns {

	bool empty;
    vector<regex> items;

    FilterPatterns(reader::Corpus<T>* reader, bool excluding = false, bool dropDuplicates = true, bool verbose = false) {
        try {
            items = reader->readFilterPatterns(excluding, dropDuplicates, verbose); 
            empty = false;
        } catch (invalid_argument& e) {
            empty = true;
        }
    }

    bool match(Triple triple) {
        for (auto &filter_pattern: items) {
            if (regex_match(triple.as_filterable_string(), filter_pattern)) {
                return true;
            }
        }
        return false;
    }

};

#endif
