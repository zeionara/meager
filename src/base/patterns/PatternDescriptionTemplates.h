#ifndef PATTERNS_PATTERN_DESCRIPTION_TEMPLATES_H
#define PATTERNS_PATTERN_DESCRIPTION_TEMPLATES_H

#include "main.h"

#include "../triple/list/TripleList.h"
#include "../storage/reader/CorpusReader.h"
#include "../triple/TripleIndex.h"

#define invalidArgument invalid_argument
#define Map unordered_map

using namespace meager::main::storage;

struct PatternDescriptionTemplate {

    Pattern pattern;
    bool empty;

    PatternDescriptionTemplate(Pattern pattern) {
        this->pattern = pattern;
        this->empty = false;
    }

};

template <typename T>
struct BinaryPatternDescriptionTemplate: PatternDescriptionTemplate {

    BinaryPatternRelationMap<T>* content;

    BinaryPatternDescriptionTemplate(Pattern pattern, reader::Corpus<T>* reader, TripleEncoder<T>* encoder, bool enableFilters = false, bool verbose = false): PatternDescriptionTemplate(pattern) {
        try {
            content = reader->readBinaryPatterns(pattern, encoder, enableFilters, verbose);
        } catch (invalidArgument&) {
            empty = true;
        }
    }

};

template <typename T>
struct UnaryPatternDescriptionTemplate: PatternDescriptionTemplate {

    UnaryPatternRelationSet<T>* content;

    UnaryPatternDescriptionTemplate(Pattern pattern, reader::Corpus<T>* reader, TripleEncoder<T>* encoder, bool enableFilters = false, bool verbose = false): PatternDescriptionTemplate(pattern) {
        try {
            content = reader->readUnaryPatterns(pattern, encoder, enableFilters, verbose);
        } catch (invalidArgument&) {
            empty = true;
        }
    }

};

template <typename T>
struct PatternDescriptionTemplates {

    BinaryPatternDescriptionTemplate<T>* inverse;
    UnaryPatternDescriptionTemplate<T>* symmetric;
    
    PatternDescriptionTemplates(reader::Corpus<T>* reader, TripleEncoder<T>* encoder, bool enableFilters = true, bool verbose = false) {
        inverse = new BinaryPatternDescriptionTemplate<T>(::Pattern::inverse, reader, encoder, enableFilters, verbose);
        symmetric = new UnaryPatternDescriptionTemplate<T>(::Pattern::symmetric, reader, encoder, enableFilters, verbose);
    }

};

#endif
