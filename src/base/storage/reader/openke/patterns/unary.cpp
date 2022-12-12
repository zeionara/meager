#include "../OpenKECorpusReader.h"

namespace meager::main::storage::reader::openke {

    triple::pattern::relation::UnarySet<INT>* Corpus::readUnaryPatterns(triple::pattern::Pattern pattern, triple::Encoder<INT>* encoder, bool enableFilters, bool verbose) {
        string relativePath;

        switch (pattern) {
            case triple::pattern::Pattern::Symmetric:
                relativePath = SYMMETRIC_PATTERNS_FILENAME;
                break;
            default:
                cerr << "Pattern is not unary" << endl;
                throw invalidArgument("Pattern is not unary");
        }

        File file = File(path + relativePath, verbose);

        INT relation;
        unordered_set<INT> relations;

        while (file.good()) {
            file >> relation;

            if (verbose) {
                printf("Read relation %ld.\n", relation);
            }

            relations.insert(enableFilters ? encoder->relation->encode(relation) : relation);
        }

        if (verbose) {
            printf("Number of symmetric relations = %d.\n", (int)relations.size());
        }

        if (verbose) {
            cout << "Symmetric triples" << endl;
        }

        return new triple::pattern::relation::UnarySet<INT>(relations);
    }

}
