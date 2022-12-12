#include "OpenKECorpusReader.h"

namespace meager::main::storage::reader::openke {

    triple::list::List* Corpus::readTriples(
        SubsetType subsetType, triple::Index* tripleIndex, triple::Component tripleComponent, triple::filter::Filter<INT>* filter, triple::Encoder<INT>* encoder, bool enableFilters, bool verbose
    ) {
        FileWithHeader file = FileWithHeader(
            path + (
                subsetType == SubsetType::train ? TRAIN_FILENAME :
                subsetType == SubsetType::test ? TEST_FILENAME :
                subsetType == SubsetType::valid ? VALID_FILENAME :
                throw invalidArgument("Unknown subset type")
            ), verbose
        );

        triple::list::List* triples = new triple::list::List(file.length, tripleComponent);
        triple::Triple* items = triples->items;

        INT h = 0, r = 0, t = 0;
        INT j = 0;

        if (verbose) {
            cout << "For each of " << file.length << " triples" << endl;
        }

        for (INT i = 0; i < file.length; i++) { // Reading train samples

            file >> h >> t >> r;

            if (!enableFilters || filter->allows(triple::Triple(h, r, t))) {
                if (enableFilters) {
                    items[j].h = encoder->entity->encode(h);
                    items[j].t = encoder->entity->encode(t);
                    items[j].r = encoder->relation->encode(r);
                } else {
                    items[j].h = h;
                    items[j].t = t;
                    items[j].r = r;
                }

                tripleIndex->push(items[j]);

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
