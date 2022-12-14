#ifndef API_CORPUS_H
#define API_CORPUS_H

#include <cstring>
#include <cstdio>
#include <string>

#include "../corpus/local/Default.h"


namespace meager::main::api::corpus {

    extern
    main::corpus::local::Default<INT>* corpus;

    extern
    void init(char *path, bool enableFilters = false, bool verbose = false);

    extern
    INT countEntities(bool verbose = false); 

    extern
    INT countRelations(bool verbose = false); 

    extern
    INT countTriples(subset::Type subset, bool verbose = false);

    extern
    INT countTriples(bool verbose = false); 

    extern
    void importFilter(bool dropDuplicates = true, bool verbose = false);

    extern
    void importPattern(bool verbose = false);

    extern
    void importTrain(bool dropPatternDuplicates = true, bool verbose = false);

    extern
    void importTriples(subset::Type subset, bool verbose = false);

    extern
    void importTypes(bool verbose = false);

}

#endif
