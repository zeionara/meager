#ifndef FILTERS_FILTER_PATTERNS_H
#define FILTERS_FILTER_PATTERNS_H

#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <unordered_set>

#include "../triple/main.h"
#include "../storage/CorpusReader.h"

using namespace std;

template <typename T>
struct FilterPatterns {

	bool empty;
    vector<regex> items;

    FilterPatterns(CorpusReader<T>* reader, bool excluding = false, bool verbose = false, bool drop_duplicates = true) {
        try {
            items = reader->readFilterPatterns(excluding, verbose, drop_duplicates); 
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

    // vector<regex> readFilterPatterns(string path, bool verbose, bool drop_duplicates) {
    //     if (verbose) {
    //         cout << "Reading " << path << " triple patterns..." << endl;
    //     }

    //     ifstream in_file(path);

    //     if (!in_file.good()) {
    //         throw invalid_argument("Cannot find file " + path + " on disk");
    //     }

    //     vector<regex> patterns;

    //     for (string line; getline(in_file, line);) {
    //         if (drop_duplicates) {
    //             unordered_set<string> seenPatterns;

    //             if (seenPatterns.find(line) == seenPatterns.end()) {
    //                 seenPatterns.insert(line);
    //                 regex line_regex(line);
    //                 patterns.push_back(line_regex); 
    //                 cout << "Read filtering pattern " << line << endl;
    //             }
    //         } else {
    //             regex line_regex(line);
    //             patterns.push_back(line_regex); 
    //         }
    //     }

    //     return patterns;
    // }
};

#endif
