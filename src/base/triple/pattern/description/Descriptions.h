#ifndef TRIPLE_PATTERN_DESCRIPTION_DESCRIPTIONS_H
#define TRIPLE_PATTERN_DESCRIPTION_DESCRIPTIONS_H

// #include "../main.h"

// #include "../../list/List.h"
// #include "../../../storage/reader/CorpusReader.h"
// #include "../../Index.h"

#include "../none/Description.h"
#include "../inverse/Description.h"
#include "../symmetric/Description.h"
// #include "Templates.h"

#define invalidArgument invalid_argument

namespace meager::main::triple::pattern::description {

    template <typename T>
    struct Descriptions {
        unordered_map<Pattern, Description> content;

        Descriptions(
            triple::list::List* triples, Templates<T>* templates, triple::Index* index, triple::Encoder<T>* encoder,
            bool dropDuplicates = true, bool enableFilters = false, bool verbose = false
        ) {
            content[Pattern::None] = NonePatternDescription(triples, verbose, true);
            if (!templates->inverse->empty) {
                content[Pattern::Inverse] = inverse::Description<T>(triples, templates->inverse, index, encoder, dropDuplicates, enableFilters, verbose);
            }
            if (!templates->symmetric->empty) {
                content[Pattern::Symmetric] = symmetric::Description<T>(triples, templates->symmetric, index, encoder, dropDuplicates, enableFilters, verbose);
            }
        }

        Description get(Pattern pattern) {
            auto mapIterator = content.find(pattern);

            if (mapIterator == content.end()) {
                cout << "Cannot get pattern " << getName(pattern) << " because it is not set" << endl;
                throw invalidArgument("pattern is not set");
            }

            return mapIterator->second;
        }
    };

}

#endif
