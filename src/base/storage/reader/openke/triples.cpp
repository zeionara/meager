#include "OpenKECorpusReader.h"

namespace meager::main::storage::reader::openke {

    TripleList* Corpus::readTriples(
        SubsetType subsetType, TripleIndex* tripleIndex, TripleElement tripleElement, TripleFilter<INT>* filter, TripleEncoder<INT>* encoder, bool enableFilters, bool verbose
    ) {
        FileWithHeader file = FileWithHeader(
            path + (
                subsetType == SubsetType::train ? TRAIN_FILENAME :
                subsetType == SubsetType::test ? TEST_FILENAME :
                subsetType == SubsetType::valid ? VALID_FILENAME :
                throw invalidArgument("Unknown subset type")
            ), verbose
        );

        TripleList* triples = new TripleList(file.length, tripleElement);
        Triple* items = triples->items;

        INT h = 0, r = 0, t = 0;
        INT j = 0;

        if (verbose) {
            cout << "For each of " << file.length << " triples" << endl;
        }

        for (INT i = 0; i < file.length; i++) { // Reading train samples

            file >> h >> t >> r;

            if (!enableFilters || filter->allows(Triple(h, r, t))) {
                if (enableFilters) {
                    items[j].h = encoder->entity->encode(h);
                    items[j].t = encoder->entity->encode(t);
                    items[j].r = encoder->relation->encode(r);
                } else {
                    items[j].h = h;
                    items[j].t = t;
                    items[j].r = r;
                }

                tripleIndex->add(items[j]);

                j++;
            }
        }

        if (enableFilters && verbose) {
            cout << "Current entity id in encoder = " << encoder->entity->nEncodedValues << endl;
        }

        triples->length = j;

        return triples;
    }

}
