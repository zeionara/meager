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

    // cout << types->get(8)->tails->items[5] << endl;

    // cout << "====================================================================================" << endl;

    // File* file = readNumberOfTypeConstrainedRelations(verbose);

    // FILE* f_type = file->file;
    // INT nTypeConstrainedRelations = file->length;

	// // FILE *f_type = fopen((inPath + "type_constrain.txt").c_str(), "r");

	// // if (f_type == nullptr) {
	// // 	std::cout << '`' << inPath << "type_constrain.txt" << '`'
	// // 	          << " does not exist" << std::endl;
	// // 	return;
	// // }

    // // INT nTypeConstrainedRelations;
	// // INT tmp = fscanf(f_type, "%ld", &nTypeConstrainedRelations);
    // INT tmp;

	// INT total_lef = 0;
	// INT total_rig = 0;

	// for (INT i = 0; i < nTypeConstrainedRelations; i++) { // Count total number of entities enumerated for all relations - separately for head and tail entries
	// 	INT rel, tot;
	// 	tmp = fscanf(f_type, "%ld %ld", &rel, &tot);
	// 	for (INT j = 0; j < tot; j++) {
	// 		tmp = fscanf(f_type, "%ld", &tmp);
	// 		total_lef++;
	// 	}
	// 	tmp = fscanf(f_type, "%ld %ld", &rel, &tot);
	// 	for (INT j = 0; j < tot; j++) {
	// 		tmp = fscanf(f_type, "%ld", &tmp);
	// 		total_rig++;
	// 	}
	// }

	// fclose(f_type);

    // cout << "Total left = " << total_lef << endl;
    // cout << "Total left = " << total_lef << endl;

	// head_lef = (INT *)calloc(nTypeConstrainedRelations, sizeof(INT));
	// head_rig = (INT *)calloc(nTypeConstrainedRelations, sizeof(INT));

	// tail_lef = (INT *)calloc(nTypeConstrainedRelations, sizeof(INT));
	// tail_rig = (INT *)calloc(nTypeConstrainedRelations, sizeof(INT));

	// head_type = (INT *)calloc(total_lef, sizeof(INT)); // Allocate space for the total amount of entity ids got from the previous loop
	// tail_type = (INT *)calloc(total_rig, sizeof(INT));

	// total_lef = 0;
	// total_rig = 0;

    // file = readNumberOfTypeConstrainedRelations(verbose);

	// // f_type = fopen((inPath + "type_constrain.txt").c_str(), "r");

	// // if (f_type == nullptr) {
	// // 	std::cout << '`' << inPath << "type_constrain.txt" << '`'
	// // 	          << " does not exist" << std::endl;
	// // 	return;
	// // }

	// // tmp = fscanf(f_type, "%ld", &tmp);

	// for (INT i = 0; i < file->length; i++) {
    //     // printf("Setting head lef, i = %ld \n", i);
	// 	INT rel, tot;

	// 	tmp = fscanf(f_type, "%ld %ld", &rel, &tot);
	// 	head_lef[rel] = total_lef; // Write initial offset of entity id sublist correspoding to a relation in the shared list to the head_lef array for a given relationship
    //     // printf("Head lef at 0 = %ld \n", head_lef[0]);
	// 	for (INT j = 0; j < tot; j++) {
	// 		tmp = fscanf(f_type, "%ld", &head_type[total_lef]);
    //         // printf("%ld %ld Some head %ld \n", j, total_lef, head_type[total_lef]);
	// 		total_lef++;
	// 	}
	// 	head_rig[rel] = total_lef; // Write last element index of entity id sublist correspoding to a relation in the shared list to the head_rig array for a given relationship
	// 	std::sort(head_type + head_lef[rel], head_type + head_rig[rel]); // Sort gathered entity ids

    //     // printf("Head lef at 0 = %ld \n", head_lef[0]);
	// 	tmp = fscanf(f_type, "%ld %ld", &rel, &tot); // Repeat the same procedure for the tail list of entities mentioned in the triple tail for a given relationship
	// 	tail_lef[rel] = total_rig;

	// 	for (INT j = 0; j < tot; j++) {
	// 		tmp = fscanf(f_type, "%ld", &tail_type[total_rig]);
	// 		total_rig++;
	// 	}
	// 	tail_rig[rel] = total_rig;
	// 	std::sort(tail_type + tail_lef[rel], tail_type + tail_rig[rel]);
	// }
    // // printf("Head lef at 0 = %ld \n", head_lef[0]);
	// fclose(f_type);
}

