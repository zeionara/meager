#ifndef PATTERNS_PATTERN_DESCRIPTIONS_H
#define PATTERNS_PATTERN_DESCRIPTIONS_H

#include "main.h"

#include "../triple/list/TripleList.h"
#include "../storage/reader/CorpusReader.h"
#include "../triple/TripleIndex.h"

#include "none/NonePatternDescription.h"
#include "inverse/InversePatternDescription.h"
#include "symmetric/SymmetricPatternDescription.h"
#include "PatternDescriptionTemplates.h"

#define invalidArgument invalid_argument

template <typename T>
struct PatternDescriptions {
    unordered_map<Pattern, PatternDescription> content;

    PatternDescriptions(TripleList* triples, PatternDescriptionTemplates<T>* templates, TripleIndex* index, TripleEncoder<T>* encoder, bool dropDuplicates = true, bool enableFilters = false, bool verbose = false) {
        content[none] = NonePatternDescription(triples, verbose, true);
        if (!templates->inverse->empty) {
            content[inverse] = InversePatternDescription<T>(triples, templates->inverse, index, encoder, dropDuplicates, enableFilters, verbose);
        }
        if (!templates->symmetric->empty) {
            content[symmetric] = SymmetricPatternDescription<T>(triples, templates->symmetric, index, encoder, dropDuplicates, enableFilters, verbose);
        }
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
