#include "../corpus/local/Default.h"
#include "../corpus/reader/openke/Reader.h"

// using namespace meager::main::utils;
// using namespace meager::main::corpus;

namespace meager::main::api::corpus {

    // Init

    main::corpus::local::Default<INT>* corpus;

    bool constexpr DROP_PATTERN_DUPLICATES = true;

    void init(char *path, bool enableFilters, bool verbose) {
        corpus::corpus = new main::corpus::local::Default<INT>(new main::corpus::reader::openke::Corpus(path), enableFilters, verbose);
        if (verbose) {
            printf("corpus path is set to %s\n", path);
        }
    }

    // Read

    void importTrain(bool dropPatternDuplicates = true, bool verbose = false) {
        corpus::corpus->importTrain(dropPatternDuplicates, verbose); 
    }

    void importTriples(subset::Type subset, bool verbose) {
        switch (subset) {
            case subset::Type::train:
                corpus::corpus->importTrain(DROP_PATTERN_DUPLICATES, verbose); 
                return;
            case subset::Type::test:
                corpus::corpus->importTest(verbose); 
                return;
            case subset::Type::valid:
                corpus::corpus->importValid(verbose); 
                return;
            default:
                throw invalidArgument("Unsupported subset type given. Cannot import triples");
        }
    }

    void importPattern(bool verbose = false) {
        corpus::corpus->importPattern(verbose); 
    }

    void importFilter(bool dropDuplicates = true, bool verbose = false) {
        corpus::corpus->importFilter(dropDuplicates, verbose); 
    }

    void importTypes(bool verbose = false) {
        corpus::corpus->importTypes(verbose); 
    }

    // Get

    long countEntities(bool verbose) {
        return corpus::corpus->countEntities();
    }

    long countRelations(bool verbose) {
        return corpus::corpus->countRelations();
    }

    long countTriples(subset::Type subset, bool verbose) {
        switch (subset) {
            case subset::Type::train:
                if (corpus::corpus->train == nullptr) {
                    throw invalidArgument(main::corpus::local::TRAIN_IS_NOT_INITIALIZED);
                }
                return corpus::corpus->train->length;
            case subset::Type::test:
                if (corpus::corpus->test == nullptr) {
                    throw invalidArgument(main::corpus::local::TEST_IS_NOT_INITIALIZED);
                }
                return corpus::corpus->test->length;
            case subset::Type::valid:
                if (corpus::corpus->valid == nullptr) {
                    throw invalidArgument(main::corpus::local::VALID_IS_NOT_INITIALIZED);
                }
                return corpus::corpus->valid->length;
            default:
                throw invalidArgument("Unsupported subset type given. Cannot count triples");
        }
    }

    long countTriples(bool verbose) {
        // return countTrainTriples() + countTestTriples() + countValidTriples();
        return corpus::corpus->getLength();
    }

}
