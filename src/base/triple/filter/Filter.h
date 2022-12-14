#ifndef TRIPLE_FILTER_FILTER_H
#define TRIPLE_FILTER_FILTER_H

#include "Patterns.h"

// using namespace meager::main::storage;

namespace meager::main::triple::filter {

    template <typename T>
    struct Filter {

        Patterns<T>* excluding;
        Patterns<T>* including;

        Filter(corpus::reader::Corpus<T>* reader, bool dropDuplicates, bool verbose) {
            if (verbose) {
                cout << "started initializing filter" << endl;
            }

            including = new Patterns<T>(reader, false, dropDuplicates, verbose);
            excluding = new Patterns<T>(reader, true, dropDuplicates, verbose);

            if (verbose) {
                cout << "finished initializing filter" << endl;
            }
        }

        bool allows(triple::Triple triple) {
            return (
                (including->empty || including->match(triple)) && 
                (excluding->empty || !excluding->match(triple))
            );
        }
    };

}

#endif
