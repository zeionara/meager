#include "OpenKECorpusReader.h"

namespace meager::main::storage::reader::openke {

    RelationTypesContents<INT>* Corpus::readRelationTypesContents(bool verbose) {
        FileWithHeader file = FileWithHeader(path + TYPE_FILENAME, verbose);

        RelationTypeContents<INT>** relations = (RelationTypeContents<INT>**)calloc(file.length * 2, sizeof(RelationTypeContents<INT>*));

        for (INT i = 0; i < file.length * 2; i++) {
            INT relation = 0, length = 0;

            file >> relation >> length;

            INT* items = (INT*)calloc(length, sizeof(INT));

            for (INT j = 0; j < length; j++) {
                file >> items[j];
            }

            relations[i] = new RelationTypeContents<INT>(items, length, relation);
        }

        return new RelationTypesContents<INT>(relations, file.length * 2);
    }

}
