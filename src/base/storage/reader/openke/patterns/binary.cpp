#include "../OpenKECorpusReader.h"

namespace meager::main::storage::reader::openke {

    BinaryPatternRelationMap<INT>* Corpus::readBinaryPatterns(Pattern pattern, triple::Encoder<INT>* encoder, bool enableFilters, bool verbose) {
        string relativePath;

        switch (pattern) {
            case inverse:
                relativePath = INVERSE_PATTERNS_FILENAME;
                break;
            default:
                cerr << "Pattern is not binary" << endl;
                throw invalidArgument("Pattern is not binary");
        }

        File file = File(path + relativePath, verbose);

        INT firstRelation, secondRelation;

        unordered_map<INT, INT> firstRelationToSecond;
        unordered_map<INT, INT> secondRelationToFirst;

        while (file.good()) {
            file >> firstRelation >> secondRelation;

            if (verbose) {
                printf("Read relations %ld and %ld.\n", firstRelation, secondRelation);
            }

            firstRelationToSecond[firstRelation] = enableFilters ? encoder->relation->encode(secondRelation) : secondRelation;
            secondRelationToFirst[secondRelation] = enableFilters ? encoder->relation->encode(firstRelation) : firstRelation;
        }

        if (verbose) {
            printf("Number of relations in binary pattern = %d.\n", (int)firstRelationToSecond.size());
        }

        return new BinaryPatternRelationMap<INT>(firstRelationToSecond, secondRelationToFirst);
    }

}
