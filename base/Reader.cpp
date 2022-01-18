#include "Setting.h"
#include "Triple.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

#include "TripleIndex.h"
#include "patterns/symmetric/reader.h"
#include "patterns/none/reader.h"
#include "patterns/inverse/reader.h"

// #include "filters/reader.h"
#include "filters/main.h"

INT *freqRel, *freqEnt;
INT *lefHead, *rigHead;
INT *lefTail, *rigTail;
INT *lefRel, *rigRel;
REAL *left_mean, *right_mean;

Triple *trainList;
Triple *trainHead;
Triple *trainTail;
Triple *trainRel;

INT *testLef, *testRig;
INT *validLef, *validRig;

FilterPatternsCollection* exclusionFilterPatterns;
FilterPatternsCollection* inclusionFilterPatterns;

void print_triples(std::string header, Triple* triples, int nTriples) {
    std::cout << header << "\n";
    for (int i = 0; i < nTriples; i++) {
        triples[i].print();
    }
}

// struct PatternOccurrence {
//     Triple* triple;
// }

TripleIndex* trainTripleIndex = new TripleIndex;

extern "C"
void importFilterPatterns(bool verbose, bool drop_duplicates) {
    exclusionFilterPatterns = new FilterPatternsCollection("excluding", verbose, drop_duplicates); // readFilterPatterns("excluding");
    inclusionFilterPatterns = new FilterPatternsCollection("including", verbose, drop_duplicates); // readFilterPatterns("excluding");
}

bool isAcceptableTriple(INT h, INT r, INT t) {
    Triple triple = Triple(h, r, t);
    
    return (
            (!inclusionFilterPatterns->found || doesMatchSomeFilterPatterns(*inclusionFilterPatterns, triple)) && 
            (!exclusionFilterPatterns->found || !doesMatchSomeFilterPatterns(*exclusionFilterPatterns, triple))
           );
}

extern "C"
void importTrainFiles(bool verbose = false) {

    if (verbose) {
        printf("The toolkit is importing datasets.\n");
    } 


    cout << "Read " << exclusionFilterPatterns->items.size() << " exclusion filter patterns" << endl;
    cout << "Read " << inclusionFilterPatterns->items.size() << " inclusion filter patterns" << endl;

	FILE *input_file;
	int tmp;

	input_file = fopen((inPath + "relation2id.txt").c_str(), "r");

	if (input_file == nullptr) {
		std::cout << '`' << inPath << "relation2id.txt" << '`' << " does not exist"
		          << std::endl;
		return;
	}

	tmp = fscanf(input_file, "%ld", &relationTotal);
    if (verbose) {
        printf("The total of relations is %ld.\n", relationTotal);
    }

	fclose(input_file);

	input_file = fopen((inPath + "entity2id.txt").c_str(), "r");

	if (input_file == nullptr) {
		std::cout << '`' << inPath << "entity2id.txt" << '`' << " does not exist"
		          << std::endl;
		return;
	}

	tmp = fscanf(input_file, "%ld", &entityTotal);
    if (verbose) {
        printf("The total of entities is %ld.\n", entityTotal);
    }

	fclose(input_file);

	input_file = fopen((inPath + "train2id.txt").c_str(), "r");

	if (input_file == nullptr) {
		std::cout << '`' << inPath << "train2id.txt" << '`' << " does not exist"
		          << std::endl;
		return;
	}

	tmp = fscanf(input_file, "%ld", &trainTotal); // Reading number of train samples
	trainList = (Triple *)calloc(trainTotal, sizeof(Triple));
	trainHead = (Triple *)calloc(trainTotal, sizeof(Triple));
	trainTail = (Triple *)calloc(trainTotal, sizeof(Triple));
	trainRel = (Triple *)calloc(trainTotal, sizeof(Triple));
	freqRel = (INT *)calloc(relationTotal, sizeof(INT));
	freqEnt = (INT *)calloc(entityTotal, sizeof(INT));
    // std::cout << trainList << " | " << trainHead;

    INT j = 0;

	for (INT i = 0; i < trainTotal; i++) { // Reading train samples
        INT h, r, t;

		tmp = fscanf(input_file, "%ld", &h);
		tmp = fscanf(input_file, "%ld", &t);
		tmp = fscanf(input_file, "%ld", &r);

        // Triple triple = Triple(h, r, t);

        if (isAcceptableTriple(h, r, t)) {
            trainList[j].h = h;
            trainList[j].t = t;
            trainList[j].r = r;

            trainTripleIndex->add(trainList[j]);

            j++;
        }

        // std::cout << "Current train triple: " << trainList[i].as_filterable_string() << "; matches an exclusion pattern: " << doesMatchSomeFilterPatterns(exclusionFilterPatterns, trainList[i]) << endl;
	}

    trainTotal = j;

    cout << "TRAIN TOTAL (1) = " << trainTotal;

    // cout << trainTripleIndex->contains(Triple(999, 1, 8)) << endl;

    // for (int i = 0; i < trainTotal; i++) {
    //     trainList[i].print();
    // }

    separateNoneTriples(verbose);
    separateSymmetricTriples(verbose);
    separateInverseTriples(verbose);

    // print_triples("Train triples", trainList, trainTotal);
    // print_triples("Train triples (head)", trainHead, trainTotal);

	fclose(input_file);
	std::sort(trainList, trainList + trainTotal, Triple::cmp_head);
	tmp = trainTotal;
	trainTotal = 1;
	trainHead[0] = trainTail[0] = trainRel[0] = trainList[0];
	freqEnt[trainList[0].t] += 1;
	freqEnt[trainList[0].h] += 1;
	freqRel[trainList[0].r] += 1;
	for (INT i = 1; i < tmp; i++) // Remove duplicated triples
		if (trainList[i].h != trainList[i - 1].h ||
		    trainList[i].r != trainList[i - 1].r ||
		    trainList[i].t != trainList[i - 1].t) {
			trainHead[trainTotal] = trainTail[trainTotal] = trainRel[trainTotal] =
			    trainList[trainTotal] = trainList[i];
			trainTotal++;
			freqEnt[trainList[i].t]++;
			freqEnt[trainList[i].h]++;
			freqRel[trainList[i].r]++;
		}

	std::sort(trainHead, trainHead + trainTotal, Triple::cmp_head);
	std::sort(trainTail, trainTail + trainTotal, Triple::cmp_tail);
	std::sort(trainRel, trainRel + trainTotal, Triple::cmp_rel);
    if (verbose) {
        printf("The total of train triples is %ld.\n", trainTotal);
    }

	lefHead = (INT *)calloc(entityTotal, sizeof(INT));
	rigHead = (INT *)calloc(entityTotal, sizeof(INT));
	lefTail = (INT *)calloc(entityTotal, sizeof(INT));
	rigTail = (INT *)calloc(entityTotal, sizeof(INT));
	lefRel = (INT *)calloc(entityTotal, sizeof(INT));
	rigRel = (INT *)calloc(entityTotal, sizeof(INT));
	memset(rigHead, -1, sizeof(INT) * entityTotal);
	memset(rigTail, -1, sizeof(INT) * entityTotal);
	memset(rigRel, -1, sizeof(INT) * entityTotal);
	for (INT i = 1; i < trainTotal; i++) { // For each element in any triple position found a (closed) interval in the list of sorted triples in which entries with this element occur
		if (trainTail[i].t != trainTail[i - 1].t) { // lef - left boundary of such an interval, rig - right boundary
			rigTail[trainTail[i - 1].t] = i - 1;
			lefTail[trainTail[i].t] = i;
		}
		if (trainHead[i].h != trainHead[i - 1].h) {
			rigHead[trainHead[i - 1].h] = i - 1;
			lefHead[trainHead[i].h] = i;
		}
		if (trainRel[i].h != trainRel[i - 1].h) {
			rigRel[trainRel[i - 1].h] = i - 1;
			lefRel[trainRel[i].h] = i;
		}
	}
	lefHead[trainHead[0].h] = 0;
	rigHead[trainHead[trainTotal - 1].h] = trainTotal - 1;
	lefTail[trainTail[0].t] = 0;
	rigTail[trainTail[trainTotal - 1].t] = trainTotal - 1;
	lefRel[trainRel[0].h] = 0;
	rigRel[trainRel[trainTotal - 1].h] = trainTotal - 1;

	left_mean = (REAL *)calloc(relationTotal, sizeof(REAL));
	right_mean = (REAL *)calloc(relationTotal, sizeof(REAL));
	for (INT i = 0; i < entityTotal; i++) {
		for (INT j = lefHead[i] + 1; j <= rigHead[i]; j++)
			if (trainHead[j].r != trainHead[j - 1].r) // Count number of triples in which a given head entity is occurred skipping ones in which the relationship is repeating
				left_mean[trainHead[j].r] += 1.0; // because since the relationship is the same and the head is the same, then the tail entity must be different, but tail entities
		if (lefHead[i] <= rigHead[i]) // will be considered in the following loop
			left_mean[trainHead[lefHead[i]].r] += 1.0; // Consider case of interval which consists of just one triple (in these circumstances the loop above won't execute)
		for (INT j = lefTail[i] + 1; j <= rigTail[i]; j++)
			if (trainTail[j].r != trainTail[j - 1].r) // The same as for the head entities in the previous loop - skip ones in which head entity is unique
				right_mean[trainTail[j].r] += 1.0;
		if (lefTail[i] <= rigTail[i])
			right_mean[trainTail[lefTail[i]].r] += 1.0;
	}
	for (INT i = 0; i < relationTotal; i++) {
		left_mean[i] = freqRel[i] / left_mean[i]; // Compute portion of triples in which some relationship is presented across all triples using number of triples which discards elements in which
		right_mean[i] = freqRel[i] / right_mean[i]; // only the opposite item (for lef - tail and for rig - head) is unique.
	}
}

Triple *testList;
Triple *validList;
Triple *tripleList;

extern "C"
void importTestFiles(bool verbose = false) {
	FILE *fin;
	INT tmp;

	fin = fopen((inPath + "relation2id.txt").c_str(), "r");
	if (fin == nullptr) {
		std::cout << '`' << inPath << "relation2id.txt" << '`' << " does not exist"
		          << std::endl;
		return;
	}

	tmp = fscanf(fin, "%ld", &relationTotal); // Read number of relationships
	fclose(fin);

	fin = fopen((inPath + "entity2id.txt").c_str(), "r");
	if (fin == nullptr) {
		std::cout << '`' << inPath << "entity2id.txt" << '`' << " does not exist"
		          << std::endl;
		return;
	}

	tmp = fscanf(fin, "%ld", &entityTotal); // Read number of entities
	fclose(fin);

	FILE *f_kb1 = fopen((inPath + "test2id.txt").c_str(), "r");

	if (f_kb1 == nullptr) {
		std::cout << '`' << inPath << "test2id.txt" << '`' << " does not exist"
		          << std::endl;
		return;
	}

	FILE *f_kb2 = fopen((inPath + "train2id.txt").c_str(), "r");
	if (f_kb2 == nullptr) {
		std::cout << '`' << inPath << "train2id.txt" << '`' << " does not exist"
		          << std::endl;
		return;
	}

	FILE *f_kb3 = fopen((inPath + "valid2id.txt").c_str(), "r");
	if (f_kb3 == nullptr) {
		std::cout << '`' << inPath << "valid2id.txt" << '`' << " does not exist"
		          << std::endl;
		return;
	}

	tmp = fscanf(f_kb1, "%ld", &testTotal);
	tmp = fscanf(f_kb2, "%ld", &trainTotal);
	tmp = fscanf(f_kb3, "%ld", &validTotal);
	tripleTotal = testTotal + trainTotal + validTotal;
	testList = (Triple *)calloc(testTotal, sizeof(Triple));
	validList = (Triple *)calloc(validTotal, sizeof(Triple));
	tripleList = (Triple *)calloc(tripleTotal, sizeof(Triple));

    INT j = 0;
	for (INT i = 0; i < testTotal; i++) { // Read test triples, copy each triple into the begging of the tripleList array
        INT h, t, r;

		tmp = fscanf(f_kb1, "%ld", &h);
		tmp = fscanf(f_kb1, "%ld", &t);
		tmp = fscanf(f_kb1, "%ld", &r);

        if (isAcceptableTriple(h, r, t)) {
            testList[j].h = h;
            testList[j].t = t;
            testList[j].r = r;

            tripleList[j] = testList[j];

            j++;
        }
	}

    testTotal = j;
    j = 0;

	for (INT i = 0; i < trainTotal; i++) { // Read train triples into the middle of the tripleList array
        INT h, t, r;

		tmp = fscanf(f_kb2, "%ld", &h);
		tmp = fscanf(f_kb2, "%ld", &t);
		tmp = fscanf(f_kb2, "%ld", &r);

        if (isAcceptableTriple(h, r, t)) {
            tripleList[j + testTotal].h = h;
            tripleList[j + testTotal].t = t;
            tripleList[j + testTotal].r = r;

            // cout << " j = " << j << " and test total = " << testTotal << endl;
            trainList[j] = tripleList[j + testTotal];

            j++;
        }
	}

    trainTotal = j;
    j = 0;

	for (INT i = 0; i < validTotal; i++) { // Read valid triples into the end of the tripleList array, copy each triple to a separate array
        INT h, t, r;

		tmp = fscanf(f_kb3, "%ld", &h);
		tmp = fscanf(f_kb3, "%ld", &t);
		tmp = fscanf(f_kb3, "%ld", &r);

        if (isAcceptableTriple(h, r, t)) {
            tripleList[j + testTotal + trainTotal].h = h;
            tripleList[j + testTotal + trainTotal].t = t;
            tripleList[j + testTotal + trainTotal].r = r;

            validList[j] = tripleList[j + testTotal + trainTotal];

            j++;
        }
	}

    validTotal = j;
    j = 0;

	fclose(f_kb1);
	fclose(f_kb2);
	fclose(f_kb3);

	std::sort(tripleList, tripleList + tripleTotal, Triple::cmp_head);
	std::sort(testList, testList + testTotal, Triple::cmp_rel2);
	std::sort(validList, validList + validTotal, Triple::cmp_rel2);
    if (verbose) {
        printf("The total of test triples is %ld.\n", testTotal);
        printf("The total of valid triples is %ld.\n", validTotal);
    }

	testLef = (INT *)calloc(relationTotal, sizeof(INT));
	testRig = (INT *)calloc(relationTotal, sizeof(INT));
	memset(testLef, -1, sizeof(INT) * relationTotal);
	memset(testRig, -1, sizeof(INT) * relationTotal);
	for (INT i = 1; i < testTotal; i++) { // Get intervals for unique relationships in the test subset
		if (testList[i].r != testList[i - 1].r) {
			testRig[testList[i - 1].r] = i - 1;
			testLef[testList[i].r] = i;
		}
	}
    if (testTotal > 0) {
        testLef[testList[0].r] = 0;
    }
    if (testTotal > 1) {
        testRig[testList[testTotal - 1].r] = testTotal - 1;
    }

    cout << "foo" << endl;
	validLef = (INT *)calloc(relationTotal, sizeof(INT));
	validRig = (INT *)calloc(relationTotal, sizeof(INT));
	memset(validLef, -1, sizeof(INT) * relationTotal);
	memset(validRig, -1, sizeof(INT) * relationTotal);
	for (INT i = 1; i < validTotal; i++) { // Get intervals for unique relationships in the validation subset
		if (validList[i].r != validList[i - 1].r) {
			validRig[validList[i - 1].r] = i - 1;
			validLef[validList[i].r] = i;
		}
	}
	validLef[validList[0].r] = 0;
	validRig[validList[validTotal - 1].r] = validTotal - 1;
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

