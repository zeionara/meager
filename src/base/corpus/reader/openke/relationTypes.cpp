#include "Reader.h"

namespace meager::main::corpus::reader::openke {

    triple::relation::TypesContents<INT>* Corpus::readRelationTypesContents(bool verbose) {
        FileWithHeader file = FileWithHeader(path + TYPE_FILENAME, verbose);

        triple::relation::TypeContents<INT>** relations = (triple::relation::TypeContents<INT>**)calloc(file.length * 2, sizeof(triple::relation::TypeContents<INT>*));

        for (INT i = 0; i < file.length * 2; i++) {
            INT relation = 0, length = 0;

            file >> relation >> length;

            INT* items = (INT*)calloc(length, sizeof(INT));

            for (INT j = 0; j < length; j++) {
                file >> items[j];
            }

            relations[i] = new triple::relation::TypeContents<INT>(items, length, relation);
        }

        return new triple::relation::TypesContents<INT>(relations, file.length * 2);
    }

}
