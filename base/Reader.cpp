#include "triple/list.h"
#include "triple/type.h"

TrainTripleLists* trainLists;
TestTripleLists* testLists;
TestTripleLists* validLists;

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
    
    trainLists = new TrainTripleLists(train, enable_filters, verbose);

    if (verbose) {
        cout << "print relation score" << endl;
        trainLists->relationScore->print();
    }
}

extern "C"
void importTestFiles(bool verbose = false, bool enable_filters = false) {
    // cout << "Starting reading test lists" << endl;
    // cout << trainLists << endl;
    testLists = new TestTripleLists(test, trainLists->frequencies->nEntities, trainLists->frequencies->nRelations, enable_filters, verbose);
    // cout << "Starting reading valid lists" << endl;
    validLists = new TestTripleLists(test, testLists->nEntities, testLists->nRelations, enable_filters, verbose);
}

RelationTypes* types;

extern "C"
void importTypeFiles() {
    bool verbose = true;

    types = new RelationTypes(verbose);

    cout << "Finished reading relation types" << endl;
}

