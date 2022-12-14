#include "Default.h"

namespace meager::main::corpus::local {

    // template <typename T>
    // void Default<T>::importPattern(bool verbose) {
    //     if (verbose) {
    //         cout << "started reading pattern description templates" << endl;
    //     }

    //     this->patterns = new triple::pattern::description::Templates<T>(this->reader, encoder, this->enableFilters, verbose);

    //     if (verbose) {
    //         cout << "finished reading pattern description templates" << endl;
    //     }
    // }

    // template <typename T>
    // void Default<T>::importTrain(bool dropPatternDuplicates, bool verbose) {
    //     if (verbose) {
    //         cout << "started reading train subset" << endl;
    //     }

    //     if (train != nullptr) {
    //         throw invalidArgument(TRAIN_IS_ALREADY_INITIALIZED);
    //     }

    //     this->train = new triple::list::ThickWrapper<T>(subset::Type::train, this->reader, filter, encoder, patterns, this->enableFilters, dropPatternDuplicates, verbose);

    //     if (verbose) {
    //         cout << "finished reading train subset" << endl;
    //     }
    // }

    // template <typename T>
    // void Default<T>::importTest(bool verbose) {
    //     if (verbose) {
    //         cout << "started reading test subset" << endl;
    //     }

    //     if (test != nullptr) {
    //         throw invalidArgument(TEST_IS_ALREADY_INITIALIZED);
    //     }

    //     this->test = new triple::list::ThinWrapper<T>(subset::Type::test, this->reader, filter, encoder, this->enableFilters, verbose);

    //     if (verbose) {
    //         cout << "finished reading test subset" << endl;
    //     }
    // }

    // template <typename T>
    // void Default<T>::importValid(bool verbose) {
    //     if (verbose) {
    //         cout << "started reading valid subset" << endl;
    //     }

    //     if (valid != nullptr) {
    //         throw invalidArgument(VALID_IS_ALREADY_INITIALIZED);
    //     }

    //     this->valid = new triple::list::ThinWrapper<T>(subset::Type::valid, this->reader, filter, encoder, this->enableFilters, verbose);

    //     if (verbose) {
    //         cout << "finished reading valid subset" << endl;
    //     }
    // }

    // template <typename T>
    // void Default<T>::importTypes(bool verbose) {
    //     if (verbose) {
    //         cout << "started reading relation types" << endl;
    //     }

    //     this->types = new triple::relation::Types<T>(this->enableFilters, encoder, this->reader, verbose);

    //     if (verbose) {
    //         cout << "finished reading " << types->length << " relation types" << endl;
    //     }
    // }

    // template <typename T>
    // void Default<T>::importFilter(bool dropDuplicates, bool verbose) {
    //     if (this->enableFilters) {
    //         this->filter = new triple::filter::Filter<T>(this->reader, dropDuplicates, verbose);
    //     } else {
    //         if (verbose) {
    //             cout << "skip reading filter because filters are disabled" << endl;
    //         }
    //     }
    // }

}
