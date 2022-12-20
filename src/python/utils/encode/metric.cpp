#include "metric.h"

namespace meager::python::utils::encode::metric {

    EncodedTree::~EncodedTree() {
        // cout << "foo" << endl;
        if (this->values != NULL) {
            for (unsigned long i = 0; i < length; i++) {
                // cout << "deleting " << i << " value" << endl;
                delete this->values[i];
            }
        }
        delete [] content;
    }

}
