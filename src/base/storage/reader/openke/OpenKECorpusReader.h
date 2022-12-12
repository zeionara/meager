#ifndef STORAGE_OPENKE_CORPUS_READER_H
#define STORAGE_OPENKE_CORPUS_READER_H

#include <string>
#include <fstream>

#include "../../../triple/filter/Filter.h"

#include "../CorpusReader.h"

#define unorderedSet unordered_set
#define pushBack push_back
#define invalidArgument invalid_argument

using namespace std;

namespace meager::main::storage::reader::openke {

    string const TRAIN_FILENAME = "train2id.txt";
    string const TEST_FILENAME = "test2id.txt";
    string const VALID_FILENAME = "valid2id.txt";

    string const TYPE_FILENAME = "type_constrain.txt";

    string const ENTITIES_FILENAME = "entity2id.txt";
    string const RELATIONS_FILENAME = "relation2id.txt";

    string const INCLUDING_FILTERS_FILENAME = "filters/including.txt";
    string const EXCLUDING_FILTERS_FILENAME = "filters/excluding.txt";

    string const INVERSE_PATTERNS_FILENAME = "patterns/inverse.txt";
    string const SYMMETRIC_PATTERNS_FILENAME = "patterns/symmetric.txt";

    struct Corpus: reader::Corpus<INT> {
        string path;

        Corpus(string path) {
            this->path = path;
        }

        triple::list::List* readTriples(
            SubsetType subsetType, triple::Index* tripleIndex, triple::Component tripleComponent, triple::filter::Filter<INT>* filter, triple::Encoder<INT>* encoder,
            bool enableFilters = false, bool verbose = false
        );

        vector<regex> readFilterPatterns(bool excluding = false, bool verbose = false, bool dropDuplicates = true);
        RelationTypesContents<INT>* readRelationTypesContents(bool verbose = false);

        triple::pattern::relation::BinaryMap<INT>* readBinaryPatterns(triple::pattern::Pattern pattern, triple::Encoder<INT>* encoder, bool enableFilters = false, bool verbose = false);
        triple::pattern::relation::UnarySet<INT>* readUnaryPatterns(triple::pattern::Pattern pattern, triple::Encoder<INT>* encoder, bool enableFilters = false, bool verbose = false);

        INT readVocabularySize(triple::ComponentType componentType, bool verbose = false) {
            return FileWithHeader(path + (componentType == triple::ComponentType::entity ? ENTITIES_FILENAME : RELATIONS_FILENAME), verbose).length;
        }

    };

}

#endif
