#ifndef PATTERNS_PATTERN_DESCRIPTIONS_H
#define PATTERNS_PATTERN_DESCRIPTIONS_H

#include "main.h"

#include "../triple/list/TripleList.h"
#include "../storage/CorpusReader.h"
#include "../triple/TripleIndex.h"

#include "none/NonePatternDescription.h"
#include "inverse/InversePatternDescription.h"
#include "symmetric/SymmetricPatternDescription.h"

template <typename T>
struct PatternDescriptions {
    unordered_map<Pattern, PatternDescription> content;

    PatternDescriptions(TripleList* triples, CorpusReader<T>* reader, TripleIndex* index, bool verbose = false, bool dropDuplicates = true) {
        content[none] = NonePatternDescription(triples, verbose, true);
        content[inverse] = InversePatternDescription<T>(triples, reader, index, verbose, true);
        content[symmetric] = SymmetricPatternDescription<T>(triples, reader, index, verbose, true);
    }

    PatternDescription get(Pattern pattern) {
        auto mapIterator = content.find(pattern);

        if (mapIterator == content.end()) {
            cout << "Pattern " << pattern << " is not set" << endl;
            throw "Pattern is not set";
        }

        return mapIterator->second;
    }
};

#endif
