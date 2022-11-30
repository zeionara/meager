#ifndef PATTERNS_PATTERN_DESCRIPTIONS_H
#define PATTERNS_PATTERN_DESCRIPTIONS_H

#include "main.h"

#include "../triple/list/TripleList.h"
#include "../storage/CorpusReader.h"
#include "../triple/TripleIndex.h"

#include "none/NonePatternDescription.h"
#include "inverse/InversePatternDescription.h"
#include "symmetric/SymmetricPatternDescription.h"

#define invalidArgument invalid_argument

template <typename T>
struct PatternDescriptions {
    unordered_map<Pattern, PatternDescription> content;

    PatternDescriptions(TripleList* triples, CorpusReader<T>* reader, TripleIndex* index, TripleEncoder<T>* encoder, bool dropDuplicates = true, bool enableFilters = false, bool verbose = false) {
        content[none] = NonePatternDescription(triples, verbose, true);
        content[inverse] = InversePatternDescription<T>(triples, reader, index, encoder, true, enableFilters, verbose);
        content[symmetric] = SymmetricPatternDescription<T>(triples, reader, index, encoder, true, enableFilters, verbose);
    }

    PatternDescription get(Pattern pattern) {
        auto mapIterator = content.find(pattern);

        if (mapIterator == content.end()) {
            cout << "Cannot get pattern " << pattern << " because it is not set" << endl;
            throw invalidArgument("pattern is not set");
        }

        return mapIterator->second;
    }
};

#endif
