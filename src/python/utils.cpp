#include "../base/sampling/batch/Triple.h"

namespace meager::python::utils::encode {

    void** tripleBatch(meager::main::sampling::batch::Triple* batch, bool includeLabels) {
        void** result = new void*[includeLabels ? 5 : 4];

        int i = 0;

        result[i++] = batch->head;
        result[i++] = batch->relation;
        result[i++] = batch->tail;

        if (includeLabels) {
            result[i++] = batch->labels;
        }

        result[i++] = (void*)batch->length;

        return result;
    }

}
