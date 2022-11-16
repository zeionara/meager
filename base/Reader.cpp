#include "Setting.h"
#include "triple/main.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

#include "triple/TripleIndex.h"
#include "triple/reader.h"
#include "patterns/symmetric/reader.h"
#include "patterns/none/reader.h"
#include "patterns/inverse/reader.h"

// #include "filters/reader.h"
#include "filters/main.h"
#include "utils/main.h"
#include "triple/list.h"
#include "triple/type.h"

INT *freqRel, *freqEnt;
INT *lefHead, *rigHead;
INT *lefTail, *rigTail;
INT *lefRel, *rigRel;
REAL *left_mean, *right_mean;

Triple *trainHead;
Triple *trainTail;
Triple *trainRel;

Triple *trainList;
Triple *testList;
Triple *validList;

Triple *tripleList;

INT *testLef, *testRig;
INT *validLef, *validRig;

// INT* current_internal_entity_id = new INT(0);
// INT* current_internal_relation_id = new INT(0);

TripleIds tripleIds;

TrainTripleLists* trainLists;
TestTripleLists* testLists;
TestTripleLists* validLists;

void print_triples(std::string header, Triple* triples, int nTriples) {
    std::cout << header << "\n";
    for (int i = 0; i < nTriples; i++) {
        triples[i].print();
    }
}

TripleIndex* trainTripleIndex = new TripleIndex;
TripleIndex* testTripleIndex = new TripleIndex;
TripleIndex* validTripleIndex = new TripleIndex;

extern "C"
void importTrainFiles(bool verbose = false, bool enable_filters = false) {

    if (verbose) {
        printf("The toolkit is importing datasets.\n");

        if (enable_filters) {
            cout << "Filters are enabled" << endl;
            cout << "Read " << exclusionFilterPatterns->items.size() << " exclusion filter patterns" << endl;
            cout << "Read " << inclusionFilterPatterns->items.size() << " inclusion filter patterns" << endl;
        }
    } 
    
    TrainTripleLists* lists = new TrainTripleLists(train, enable_filters, verbose);

    if (verbose) {
        cout << "print relation score" << endl;
        lists->relationScore->print();
    }

    trainTotal = lists->length;
    // cout << "Train total =================== " << trainTotal;
    relationTotal = lists->frequencies->nRelations;
    entityTotal = lists->frequencies->nEntities;

    trainLists = lists;

    freqRel = lists->frequencies->relation;
    freqEnt = lists->frequencies->entity;

    lefHead = lists->head->left;
    rigHead = lists->head->right;
    lefTail = lists->tail->left;
    rigTail = lists->tail->right;
    lefRel = lists->relation->left;
    rigRel = lists->relation->right;

    left_mean = lists->relationScore->head;
    right_mean = lists->relationScore->tail;

    trainList = lists->main->items;
    trainHead = lists->head->items;
    trainTail = lists->tail->items;
    trainRel = lists->relation->items;
}

extern "C"
void importTestFiles(bool verbose = false, bool enable_filters = false) {
	FILE *fin;
	INT tmp;

    testLists = new TestTripleLists(test, trainLists->frequencies->nEntities, trainLists->frequencies->nRelations, enable_filters, verbose);
    validLists = new TestTripleLists(test, testLists->nEntities, testLists->nRelations, enable_filters, verbose);

	tripleTotal = trainLists->length + testLists->length + validLists->length;

    testTotal = testLists->length;
    testList = testLists->main->items;
    testLef = testLists->main->left;
    testRig = testLists->main->right;

    validTotal = validLists->length;
    validList = validLists->main->items;
    validLef = validLists->main->left;
    validRig = validLists->main->right;
}

INT *head_lef;
INT *head_rig;
INT *tail_lef;
INT *tail_rig;
INT *head_type;
INT *tail_type;

RelationTypes* types;

extern "C"
void importTypeFiles() {
    bool verbose = true;

    types = new RelationTypes(verbose);
}

