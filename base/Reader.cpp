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
    
    // File* relations = readNumberOfElements(TripleComponent::relation, verbose);
    // File* entities = readNumberOfElements(TripleComponent::entity, verbose);

    // File* trainFile = readNumberOfTriples(train, verbose);

    // TripleLists* lists = new TripleLists(trainTotal);
    // TripleIds tripleIds = readTriples(input_file, enable_filters, lists->main, trainTripleIndex);
    // TrainTripleLists* lists = new TrainTripleLists(trainFile->length);
    TrainTripleLists* lists = new TrainTripleLists(train, enable_filters, verbose);
    // lists->read(trainFile);

    trainTotal = lists->length;
    relationTotal = lists->frequencies->nRelations;
    entityTotal = lists->frequencies->nEntities;

    // tripleIds = readTriples(trainFile, enable_filters, lists->main, trainTripleIndex);
    // tripleIds = readTriples(trainFile, enable_filters, lists->main, trainTripleIndex);

	// fclose(input_file);

    // if (enable_filters) {
    //     relationTotal = tripleIds.last_relation;
    //     entityTotal = tripleIds.last_entity;
    //     trainTotal = tripleIds.last_triple;
    // }

    // Frequencies* frequencies = dropDuplicates(lists, entityTotal, relationTotal);

    // separateNoneTriples(lists->main, lists->length, verbose, true, enable_filters);
    // separateSymmetricTriples(lists->main, lists->length, verbose);
    // separateInverseTriples(lists->main, lists->length, verbose, true, enable_filters);

    // if (verbose) {
    //     printf("The total of train triples is %ld.\n", trainTotal);
    //     printf("The total of (UNIQUE) train triples is %ld.\n", lists->length);
    // }

    // BoundaryCollection* boundaries = findBoundaries(lists, frequencies);
    // RelationScore* score = new RelationScore(boundaries, lists, frequencies);

    if (verbose) {
        cout << "print relation score" << endl;
        lists->relationScore->print();
    }

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

	// fin = fopen((inPath + "relation2id.txt").c_str(), "r");
	// if (fin == nullptr) {
	// 	std::cout << '`' << inPath << "relation2id.txt" << '`' << " does not exist"
	// 	          << std::endl;
	// 	return;
	// }

	// tmp = fscanf(fin, "%ld", &relationTotal); // Read number of relationships
	// fclose(fin);

	// fin = fopen((inPath + "entity2id.txt").c_str(), "r");
	// if (fin == nullptr) {
	// 	std::cout << '`' << inPath << "entity2id.txt" << '`' << " does not exist"
	// 	          << std::endl;
	// 	return;
	// }

	// tmp = fscanf(fin, "%ld", &entityTotal); // Read number of entities
	// fclose(fin);

    // readNumberOfElements(TripleComponent::relation, verbose);
    // readNumberOfElements(TripleComponent::entity, verbose);

    // FILE *test_input_file = readNumberOfTriples(test, verbose);
    // FILE *valid_input_file = readNumberOfTriples(valid, verbose);

	// FILE *f_kb1 = fopen((inPath + "test2id.txt").c_str(), "r");

	// if (f_kb1 == nullptr) {
	// 	std::cout << '`' << inPath << "test2id.txt" << '`' << " does not exist"
	// 	          << std::endl;
	// 	return;
	// }

	// FILE *f_kb2 = fopen((inPath + "train2id.txt").c_str(), "r");
	// if (f_kb2 == nullptr) {
	// 	std::cout << '`' << inPath << "train2id.txt" << '`' << " does not exist"
	// 	          << std::endl;
	// 	return;
	// }

	// FILE *f_kb3 = fopen((inPath + "valid2id.txt").c_str(), "r");
	// if (f_kb3 == nullptr) {
	// 	std::cout << '`' << inPath << "valid2id.txt" << '`' << " does not exist"
	// 	          << std::endl;
	// 	return;
	// }

	// tmp = fscanf(f_kb1, "%ld", &testTotal);
    // tmp = fscanf(f_kb2, "%ld", &trainTotal);
	// tmp = fscanf(f_kb3, "%ld", &validTotal);

    testLists = new TestTripleLists(test, trainLists->frequencies->nEntities, trainLists->frequencies->nRelations, enable_filters, verbose);
    validLists = new TestTripleLists(test, testLists->nEntities, testLists->nRelations, enable_filters, verbose);

    // cout << "Make test lists" << endl;

	tripleTotal = trainLists->length + testLists->length + validLists->length;

    testTotal = testLists->length;
    testList = testLists->main->items;
    testLef = testLists->main->left;
    testRig = testLists->main->right;

    validTotal = validLists->length;
    validList = validLists->main->items;
    validLef = validLists->main->left;
    validRig = validLists->main->right;
	// testList = (Triple *)calloc(testTotal, sizeof(Triple));
	// validList = (Triple *)calloc(validTotal, sizeof(Triple));
	// tripleList = (Triple *)calloc(tripleTotal, sizeof(Triple));

    // if (verbose) {
    //     printf("The total of train triples (read) is %ld.\n", trainTotal);
    // }

    // TripleList* testTripleList = new TripleList(testTotal);
    // tripleIds = readTriples(test_input_file, testTotal, enable_filters, testList, testTripleIndex, tripleIds.last_entity, tripleIds.last_relation);
    // tripleIds = readTriples(test_input_file, testTotal, enable_filters, testTripleList->items, testTripleIndex, tripleIds.last_entity, tripleIds.last_relation);

    // INT j = 0;
	// for (INT i = 0; i < testTotal; i++) { // Read test triples, copy each triple into the begging of the tripleList array
    //     INT h, t, r;

	// 	tmp = fscanf(f_kb1, "%ld", &h);
	// 	tmp = fscanf(f_kb1, "%ld", &t);
	// 	tmp = fscanf(f_kb1, "%ld", &r);

    //     if (!enable_filters || isAcceptableTriple(h, r, t)) {
    //         if (enable_filters) {
    //             testList[j].h = external_to_internal_id(h, current_internal_entity_id, &external_to_internal_entity_id, &internal_to_external_entity_id);
    //             testList[j].t = external_to_internal_id(t, current_internal_entity_id, &external_to_internal_entity_id, &internal_to_external_entity_id);
    //             testList[j].r = external_to_internal_id(r, current_internal_relation_id, &external_to_internal_relation_id, &internal_to_external_relation_id);
    //         } else {
    //             testList[j].h = h;
    //             testList[j].t = t;
    //             testList[j].r = r;
    //         }

    //         tripleList[j] = testList[j];

    //         j++;
    //     }
    //     tripleList[j] = testList[j];
	// }

    // testTotal = tripleIds.last_triple;

    // TripleList* validTripleList = new TripleList(validTotal);
    // // tripleIds = readTriples(valid_input_file, validTotal, enable_filters, validList, validTripleIndex, tripleIds.last_entity, tripleIds.last_relation);
    // tripleIds = readTriples(valid_input_file, validTotal, enable_filters, validTripleList->items, validTripleIndex, tripleIds.last_entity, tripleIds.last_relation);

    // validTotal = tripleIds.last_triple;

	// fclose(test_input_file);
	// // fclose(f_kb2);
	// fclose(valid_input_file);
    // 
    // // j = 0;

	// for (INT i = 0; i < testTotal; i++) { // Read train triples into the middle of the tripleList array
    //     tripleList[i] = testList[i];
	// }

	// for (INT i = 0; i < trainTotal; i++) { // Read train triples into the middle of the tripleList array
    //     tripleList[i + testTotal] = trainList[i];
	// }

	// for (INT i = 0; i < validTotal; i++) { // Read train triples into the middle of the tripleList array
    //     tripleList[i + testTotal + trainTotal] = validList[i];
	// }

	// // for (INT i = 0; i < validTotal; i++) { // Read valid triples into the end of the tripleList array, copy each triple to a separate array
    // //     INT h, t, r;

	// // 	tmp = fscanf(f_kb3, "%ld", &h);
	// // 	tmp = fscanf(f_kb3, "%ld", &t);
	// // 	tmp = fscanf(f_kb3, "%ld", &r);

    // //     if (!enable_filters || isAcceptableTriple(h, r, t)) {
    // //         if (enable_filters) {
    // //             tripleList[j + testTotal + trainTotal].h = external_to_internal_id(h, current_internal_entity_id, &external_to_internal_entity_id, &internal_to_external_entity_id);
    // //             tripleList[j + testTotal + trainTotal].t = external_to_internal_id(t, current_internal_entity_id, &external_to_internal_entity_id, &internal_to_external_entity_id);
    // //             tripleList[j + testTotal + trainTotal].r = external_to_internal_id(r, current_internal_relation_id, &external_to_internal_relation_id, &internal_to_external_relation_id);
    // //         } else {
    // //             tripleList[j + testTotal + trainTotal].h = h;
    // //             tripleList[j + testTotal + trainTotal].t = t;
    // //             tripleList[j + testTotal + trainTotal].r = r;
    // //         }

    // //         validList[j] = tripleList[j + testTotal + trainTotal];

    // //         j++;
    // //     }
	// // }

    // // if (enable_filters) {
    // //     relationTotal = *current_internal_relation_id;
    // //     entityTotal = *current_internal_entity_id;
    // // }

    // if (enable_filters) {
    //     relationTotal = tripleIds.last_relation;
    //     entityTotal = tripleIds.last_entity;
    // }

    // // validTotal = j;
    // // j = 0;

    // testTripleList->sort(relationTotal);
    // validTripleList->sort(relationTotal);

    // testList = testTripleList->items;
    // testLef = testTripleList->left;
    // testRig = testTripleList->right;

    // validList = validTripleList->items;
    // validLef = validTripleList->left;
    // validRig = validTripleList->right;

	// // std::sort(tripleList, tripleList + tripleTotal, Triple::cmp_head);
	// // std::sort(testList, testList + testTotal, Triple::cmp_rel2);
	// // std::sort(validList, validList + validTotal, Triple::cmp_rel2);
    // // if (verbose) {
    // //     printf("The total of test triples is %ld.\n", testTotal);
    // //     printf("The total of valid triples is %ld.\n", validTotal);
    // // }

	// // testLef = (INT *)calloc(relationTotal, sizeof(INT));
	// // testRig = (INT *)calloc(relationTotal, sizeof(INT));
	// // memset(testLef, -1, sizeof(INT) * relationTotal);
	// // memset(testRig, -1, sizeof(INT) * relationTotal);
	// // for (INT i = 1; i < testTotal; i++) { // Get intervals for unique relationships in the test subset
	// // 	if (testList[i].r != testList[i - 1].r) {
	// // 		testRig[testList[i - 1].r] = i - 1;
	// // 		testLef[testList[i].r] = i;
	// // 	}
	// // }
    // // if (testTotal > 0) {
    // //     testLef[testList[0].r] = 0;
    // // }
    // // if (testTotal > 1) {
    // //     testRig[testList[testTotal - 1].r] = testTotal - 1;
    // // }

	// // validLef = (INT *)calloc(relationTotal, sizeof(INT));
	// // validRig = (INT *)calloc(relationTotal, sizeof(INT));
	// // memset(validLef, -1, sizeof(INT) * relationTotal);
	// // memset(validRig, -1, sizeof(INT) * relationTotal);
	// // for (INT i = 1; i < validTotal; i++) { // Get intervals for unique relationships in the validation subset
	// // 	if (validList[i].r != validList[i - 1].r) {
	// // 		validRig[validList[i - 1].r] = i - 1;
	// // 		validLef[validList[i].r] = i;
	// // 	}
	// // }
	// // validLef[validList[0].r] = 0;
	// // validRig[validList[validTotal - 1].r] = validTotal - 1;

    // if (verbose) {
    //     printf("The total of train triples is %ld.\n", trainTotal);
    // }
}

INT *head_lef;
INT *head_rig;
INT *tail_lef;
INT *tail_rig;
INT *head_type;
INT *tail_type;

extern "C"
void importTypeFiles() {
	INT total_lef = 0;
	INT total_rig = 0;
	FILE *f_type = fopen((inPath + "type_constrain.txt").c_str(), "r");

	if (f_type == nullptr) {
		std::cout << '`' << inPath << "type_constrain.txt" << '`'
		          << " does not exist" << std::endl;
		return;
	}

    INT nTypeConstrainedRelations;
	INT tmp = fscanf(f_type, "%ld", &nTypeConstrainedRelations);

	head_lef = (INT *)calloc(nTypeConstrainedRelations, sizeof(INT));
	head_rig = (INT *)calloc(nTypeConstrainedRelations, sizeof(INT));
	tail_lef = (INT *)calloc(nTypeConstrainedRelations, sizeof(INT));
	tail_rig = (INT *)calloc(nTypeConstrainedRelations, sizeof(INT));

	for (INT i = 0; i < nTypeConstrainedRelations; i++) { // Count total number of entities enumerated for all relations - separately for head and tail entries
		INT rel, tot;
		tmp = fscanf(f_type, "%ld %ld", &rel, &tot);
		for (INT j = 0; j < tot; j++) {
			tmp = fscanf(f_type, "%ld", &tmp);
			total_lef++;
		}
		tmp = fscanf(f_type, "%ld%ld", &rel, &tot);
		for (INT j = 0; j < tot; j++) {
			tmp = fscanf(f_type, "%ld", &tmp);
			total_rig++;
		}
	}
	fclose(f_type);

	head_type = (INT *)calloc(total_lef, sizeof(INT)); // Allocate space for the total amount of entity ids got from the previous loop
	tail_type = (INT *)calloc(total_rig, sizeof(INT));
	total_lef = 0;
	total_rig = 0;
	f_type = fopen((inPath + "type_constrain.txt").c_str(), "r");

	if (f_type == nullptr) {
		std::cout << '`' << inPath << "type_constrain.txt" << '`'
		          << " does not exist" << std::endl;
		return;
	}

	tmp = fscanf(f_type, "%ld", &tmp);
	for (INT i = 0; i < nTypeConstrainedRelations; i++) {
        // printf("Setting head lef, i = %ld \n", i);
		INT rel, tot;

		tmp = fscanf(f_type, "%ld%ld", &rel, &tot);
		head_lef[rel] = total_lef; // Write initial offset of entity id sublist correspoding to a relation in the shared list to the head_lef array for a given relationship
        // printf("Head lef at 0 = %ld \n", head_lef[0]);
		for (INT j = 0; j < tot; j++) {
			tmp = fscanf(f_type, "%ld", &head_type[total_lef]);
            // printf("%ld %ld Some head %ld \n", j, total_lef, head_type[total_lef]);
			total_lef++;
		}
		head_rig[rel] = total_lef; // Write last element index of entity id sublist correspoding to a relation in the shared list to the head_rig array for a given relationship
		std::sort(head_type + head_lef[rel], head_type + head_rig[rel]); // Sort gathered entity ids

        // printf("Head lef at 0 = %ld \n", head_lef[0]);
		tmp = fscanf(f_type, "%ld%ld", &rel, &tot); // Repeat the same procedure for the tail list of entities mentioned in the triple tail for a given relationship
		tail_lef[rel] = total_rig;
		for (INT j = 0; j < tot; j++) {
			tmp = fscanf(f_type, "%ld", &tail_type[total_rig]);
			total_rig++;
		}
		tail_rig[rel] = total_rig;
		std::sort(tail_type + tail_lef[rel], tail_type + tail_rig[rel]);
	}
    // printf("Head lef at 0 = %ld \n", head_lef[0]);
	fclose(f_type);
}

