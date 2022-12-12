#ifndef TRIPLE_PATTERN_NONE_INSTANCE_H
#define TRIPLE_PATTERN_NONE_INSTANCE_H

// #include <cstdlib>

#include "../main.h"


namespace meager::main::triple::pattern::none {

    // const string name = "none";

    struct Instance: pattern::Instance {

        Instance(triple::Triple triple) {
            triples = {triple};
            observedTripleIndices = {0};
        }

        void print(string label = name) {
            cout << "--- " << label << " triples pattern instance"  << endl;
            triple::Triple triple = triples[0];
            cout << "| head = " << triple.h << ", rel = " << triple.r << ", tail = " << triple.t << getObservanceMark(0) << endl;
            cout << "---" << endl;
        }

    };

}

#endif
