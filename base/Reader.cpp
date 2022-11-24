#include "triple/list/main.h"
#include "triple/type.h"

ThickTripleListWrapper* trainList;
ThinTripleListWrapper* testList, *validList;

void print_triples(std::string header, Triple* triples, int nTriples) {
    std::cout << header << "\n";
    for (int i = 0; i < nTriples; i++) {
        triples[i].print();
    }
}

extern "C"
void importTrainFiles(bool verbose = false, bool enable_filters = false) {

    // if (verbose) {
    //     printf("The toolkit is importing datasets.\n");

    //     if (enable_filters) {
    //         cout << "Filters are enabled" << endl;
    //         cout << "Read " << exclusionFilterPatterns->items.size() << " exclusion filter patterns" << endl;
    //         cout << "Read " << inclusionFilterPatterns->items.size() << " inclusion filter patterns" << endl;
    //     }
    // } 
    
    trainList = new ThickTripleListWrapper(train, enable_filters, verbose);

    if (verbose) {
        cout << "print relation score" << endl;
        trainList->relationScore->print();
    }
}

extern "C"
void importTestFiles(bool verbose = false, bool enable_filters = false) {
    // cout << "Starting reading test lists" << endl;
    // cout << trainLists << endl;
    testList = new ThinTripleListWrapper(test, trainList->frequencies->nEntities, trainList->frequencies->nRelations, enable_filters, verbose);
    // cout << "Starting reading valid lists" << endl;
    validList = new ThinTripleListWrapper(test, trainList->frequencies->nEntities, trainList->frequencies->nRelations, enable_filters, verbose);
}

RelationTypes* types;

extern "C"
void importTypeFiles() {
    bool verbose = true;

    types = new RelationTypes(verbose);

    cout << "Finished reading relation types" << endl;
}

