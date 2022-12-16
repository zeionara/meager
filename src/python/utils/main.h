#ifndef PYTHON_UTILS_MAIN_H
#define PYTHON_UTILS_MAIN_H
// #include "../base/api/evaluation.h"

#include "../../base/sampling/batch/Triple.h"
// #include "../../base/evaluation/metric/main.h"

// #include "../../base/evaluation/metric/tracker/Count.h"
// #include "../../base/evaluation/metric/tracker/Rank.h"
// #include "../../base/evaluation/metric/tracker/ReciprocalRank.h"

namespace meager::python::utils {

    template <typename T>
    struct List {
        T* items;
        long length;

        List(T* items, long length) {
            this->items = items;
            this->length = length;
        }

        ~List() {
            delete [] items;

        }

    };

    namespace string {

        char* toDynamicCharArray(std::string str);

    }

    namespace encode {

        void** tripleBatch(meager::main::sampling::batch::Triple* batch, bool includeLabels = true);

    }

}

#endif
