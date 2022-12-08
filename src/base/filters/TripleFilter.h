#ifndef FILTERS_LOCAL_TSV_TRIPLE_FILTER_H
#define FILTERS_LOCAL_TSV_TRIPLE_FILTER_H

#include "FilterPatterns.h"

using namespace meager::main::storage;

template <typename T>
struct TripleFilter {

    FilterPatterns<T>* excluding;
    FilterPatterns<T>* including;

    TripleFilter(reader::Corpus<T>* reader, bool dropDuplicates, bool verbose) {
        if (verbose) {
            cout << "started initializing filter" << endl;
        }

        including = new FilterPatterns<T>(reader, false, dropDuplicates, verbose);
        excluding = new FilterPatterns<T>(reader, true, dropDuplicates, verbose);

        if (verbose) {
            cout << "finished initializing filter" << endl;
        }
    }

    bool allows(Triple triple) {
        return (
            (including->empty || including->match(triple)) && 
            (excluding->empty || !excluding->match(triple))
        );
    }
};

#endif
