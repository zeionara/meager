#include "metric.h"

namespace meager::python::utils::encode::metric {

    EncodedTree::~EncodedTree() {
        if (this->values != NULL) {
            for (unsigned long i = 0; i < length; i++) {
                delete this->values[i];
            }
        }
        delete [] content;
    }

}
