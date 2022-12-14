#ifndef TRIPLE_FILTER_PATTERNS_H
#define TRIPLE_FILTER_PATTERNS_H

// #include <iostream>
// #include <vector>
// #include <regex>
// #include <fstream>
// #include <unordered_set>

// #include "../main.h"
#include "../../corpus/reader/Reader.h"

// using namespace std;
// using namespace meager::main::storage;

namespace meager::main::triple::filter {

    template <typename T>
    struct Patterns {

        bool empty;
        vector<regex> items;

        Patterns(corpus::reader::Corpus<T>* reader, bool excluding = false, bool dropDuplicates = true, bool verbose = false) {
            try {
                items = reader->readFilterPatterns(excluding, dropDuplicates, verbose); 
                empty = false;
            } catch (invalid_argument& e) {
                empty = true;
            }
        }

        bool match(triple::Triple triple) {
            for (auto &filter_pattern: items) {
                if (regex_match(triple.asFilterableString(), filter_pattern)) {
                    return true;
                }
            }
            return false;
        }

    };

}

#endif
