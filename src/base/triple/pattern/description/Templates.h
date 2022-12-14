#ifndef TRIPLE_PATTERN_DESCRIPTION_TEMPLATES_H
#define TRIPLE_PATTERN_DESCRIPTION_TEMPLATES_H

#include "../../../corpus/reader/Reader.h"

#define invalidArgument invalid_argument
#define Map unordered_map

// using namespace meager::main::storage;

namespace meager::main::triple::pattern::description {

    struct Template {

        Pattern pattern;
        bool empty;

        Template(Pattern pattern) {
            this->pattern = pattern;
            this->empty = false;
        }

    };

    template <typename T>
    struct BinaryTemplate: Template {

        relation::BinaryMap<T>* content;

        BinaryTemplate(Pattern pattern, corpus::reader::Corpus<T>* reader, triple::Encoder<T>* encoder, bool enableFilters = false, bool verbose = false): Template(pattern) {
            try {
                content = reader->readBinaryPatterns(pattern, encoder, enableFilters, verbose);
            } catch (invalidArgument&) {
                empty = true;
            }
        }

    };

    template <typename T>
    struct UnaryTemplate: Template {

        relation::UnarySet<T>* content;

        UnaryTemplate(Pattern pattern, corpus::reader::Corpus<T>* reader, triple::Encoder<T>* encoder, bool enableFilters = false, bool verbose = false): Template(pattern) {
            try {
                content = reader->readUnaryPatterns(pattern, encoder, enableFilters, verbose);
            } catch (invalidArgument&) {
                empty = true;
            }
        }

    };

    template <typename T>
    struct Templates {

        BinaryTemplate<T>* inverse;
        UnaryTemplate<T>* symmetric;
        
        Templates(corpus::reader::Corpus<T>* reader, triple::Encoder<T>* encoder, bool enableFilters = true, bool verbose = false) {
            inverse = new BinaryTemplate<T>(Pattern::Inverse, reader, encoder, enableFilters, verbose);
            symmetric = new UnaryTemplate<T>(Pattern::Symmetric, reader, encoder, enableFilters, verbose);
        }

    };

}

#endif
