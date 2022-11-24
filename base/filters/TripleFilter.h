#ifndef FILTERS_LOCAL_TSV_TRIPLE_FILTER_H
#define FILTERS_LOCAL_TSV_TRIPLE_FILTER_H

#include "FilterPatterns.h"

struct TripleFilter {

    FilterPatterns* excluding;
    FilterPatterns* including;

    // bool enabled;

    // TripleFilter(bool enable, bool verbose, bool drop_duplicates, string includingPath = "", string excludingPath = "") {
    TripleFilter(bool verbose, bool drop_duplicates, string includingPath = "", string excludingPath = "") {
        excluding = new FilterPatterns(excludingPath, verbose, drop_duplicates); // readFilterPatterns("excluding");
        including = new FilterPatterns(includingPath, verbose, drop_duplicates); // readFilterPatterns("excluding");
    }

    bool allows(Triple triple) {
        return (
            (including->empty || including->match(triple)) && 
            (excluding->empty || !excluding->match(triple))
        );
    }
};

#endif
