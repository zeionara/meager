#ifndef FILTERS_LOCAL_TSV_TRIPLE_FILTER_H
#define FILTERS_LOCAL_TSV_TRIPLE_FILTER_H

#include "FilterPatterns.h"

template <typename T>
struct TripleFilter {

    FilterPatterns<T>* excluding;
    FilterPatterns<T>* including;

    // bool enabled;

    // TripleFilter(bool enable, bool verbose, bool drop_duplicates, string includingPath = "", string excludingPath = "") {
    TripleFilter(CorpusReader<T>* reader, bool verbose, bool drop_duplicates) {
        cout << "Initializing filter" << endl;
        including = new FilterPatterns<T>(reader, false, verbose, drop_duplicates); // readFilterPatterns("excluding");
        excluding = new FilterPatterns<T>(reader, true, verbose, drop_duplicates); // readFilterPatterns("excluding");
    }

    bool allows(Triple triple) {
        return (
            (including->empty || including->match(triple)) && 
            (excluding->empty || !excluding->match(triple))
        );
    }
};

#endif
