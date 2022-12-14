#ifndef DEFAULT_LOCAL_TSV_CORPUS_H
#define DEFAULT_LOCAL_TSV_CORPUS_H

#include "has.h"
#include "count.h"
#include "import.h"

#include "Corpus.h"

using namespace std;

namespace meager::main::corpus::local {

    template <typename T>
    struct Default: Corpus<T> {
        triple::list::ThickWrapper<T>* train;

        triple::list::ThinWrapper<T>* test;
        triple::list::ThinWrapper<T>* valid;

        triple::relation::Types<T>* types;
        triple::filter::Filter<T>* filter;
        triple::Encoder<T>* encoder;
        triple::pattern::description::Templates<T>* patterns;

        Default(reader::Corpus<T>* reader, bool enableFilters = false, bool verbose = false): Corpus<T>(reader, enableFilters) {
            if (enableFilters) {
                if (verbose) {
                    cout << "filters are enabled" << endl;
                }
                encoder = new triple::Encoder<INT>();
            } else {
                encoder = nullptr;
            }
            train = nullptr;
            test = nullptr;
            valid = nullptr;
            filter = nullptr;
            types = nullptr;
            patterns = nullptr;
        };

        void importTrain(bool dropPatternDuplicates, bool verbose) { importTrain_<T>(this, dropPatternDuplicates, verbose); }
        void importTest(bool verbose) { importTest_<T>(this, verbose); }
        void importValid(bool verbose) { importValid_<T>(this, verbose); }

        void importPattern(bool verbose) { importPattern_<T>(this, verbose); }
        void importTypes(bool verbose) { importTypes_<T>(this, verbose); }
        void importFilter(bool dropDuplicates, bool verbose) { importFilter_<T>(this, dropDuplicates, verbose); }

        INT countEntities() { return countEntities_<T>(this); };
        INT countRelations() { return countRelations_<T>(this); }
        INT getLength() { return getLength_<T>(this); }

        bool contains(triple::Triple triple) { return contains_<T>(this, triple); }
        bool allows(triple::Triple triple) { return allows_<T>(this, triple); }

    };

}

#endif
