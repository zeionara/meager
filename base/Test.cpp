#include "Setting.h"
#include "Reader.h"
#include "Corrupt.h"

/*=====================================================================================
link prediction
======================================================================================*/
INT lastHead = 0;
INT lastTail = 0;
REAL l1_filter_tot = 0, l1_tot = 0, r1_tot = 0, r1_filter_tot = 0, l_tot = 0, r_tot = 0, l_filter_rank = 0, l_rank = 0, l_filter_reci_rank = 0, l_reci_rank = 0;
REAL l3_filter_tot = 0, l3_tot = 0, r3_tot = 0, r3_filter_tot = 0, l_filter_tot = 0, r_filter_tot = 0, r_filter_rank = 0, r_rank = 0, r_filter_reci_rank = 0, r_reci_rank = 0;

REAL l1_filter_tot_constrain = 0, l1_tot_constrain = 0, r1_tot_constrain = 0, r1_filter_tot_constrain = 0, l_tot_constrain = 0, r_tot_constrain = 0, l_filter_rank_constrain = 0, l_rank_constrain = 0, l_filter_reci_rank_constrain = 0, l_reci_rank_constrain = 0;
REAL l3_filter_tot_constrain = 0, l3_tot_constrain = 0, r3_tot_constrain = 0, r3_filter_tot_constrain = 0, l_filter_tot_constrain = 0, r_filter_tot_constrain = 0, r_filter_rank_constrain = 0, r_rank_constrain = 0, r_filter_reci_rank_constrain = 0, r_reci_rank_constrain = 0;

// extern "C"
void initTest() {
    lastHead = 0;
    lastTail = 0;
    l1_filter_tot = 0, l1_tot = 0, r1_tot = 0, r1_filter_tot = 0, l_tot = 0, r_tot = 0, l_filter_rank = 0, l_rank = 0, l_filter_reci_rank = 0, l_reci_rank = 0;
    l3_filter_tot = 0, l3_tot = 0, r3_tot = 0, r3_filter_tot = 0, l_filter_tot = 0, r_filter_tot = 0, r_filter_rank = 0, r_rank = 0, r_filter_reci_rank = 0, r_reci_rank = 0;

    l1_filter_tot_constrain = 0, l1_tot_constrain = 0, r1_tot_constrain = 0, r1_filter_tot_constrain = 0, l_tot_constrain = 0, r_tot_constrain = 0, l_filter_rank_constrain = 0, l_rank_constrain = 0, l_filter_reci_rank_constrain = 0, l_reci_rank_constrain = 0;
    l3_filter_tot_constrain = 0, l3_tot_constrain = 0, r3_tot_constrain = 0, r3_filter_tot_constrain = 0, l_filter_tot_constrain = 0, r_filter_tot_constrain = 0, r_filter_rank_constrain = 0, r_rank_constrain = 0, r_filter_reci_rank_constrain = 0, r_reci_rank_constrain = 0;
}


// extern "C"
void getHeadBatch(INT *ph, INT *pt, INT *pr) { // Generate all possible triples for every entity which may be used as a triple head
    for (INT i = 0; i < entityTotal; i++) {
        ph[i] = i;
        pt[i] = testList[lastHead].t;
        pr[i] = testList[lastHead].r;
    }
}

// extern "C"
void getValidHeadBatch(INT *ph, INT *pt, INT *pr) { // Generate all possible triples for every entity which may be used as a triple head
    for (INT i = 0; i < entityTotal; i++) {
        ph[i] = i;
        pt[i] = validList[lastHead].t;
        pr[i] = validList[lastHead].r;
    }
}

// extern "C"
void getTailBatch(INT *ph, INT *pt, INT *pr) {
    for (INT i = 0; i < entityTotal; i++) {
        ph[i] = testList[lastTail].h;
        pt[i] = i;
        pr[i] = testList[lastTail].r;
    }
}

// extern "C"
void getValidTailBatch(INT *ph, INT *pt, INT *pr) {
    for (INT i = 0; i < entityTotal; i++) {
        ph[i] = validList[lastTail].h;
        pt[i] = i;
        pr[i] = validList[lastTail].r;
    }
}

// extern "C"
void testHead(REAL *probabilities, bool reverse) {
    INT reference_head = testList[lastHead].h;
    INT reference_tail = testList[lastHead].t;
    INT reference_rel = testList[lastHead].r;
    INT lef = head_lef[reference_rel], rig = head_rig[reference_rel]; // Left and right bound of sets of entity-ids in the common list of acceptable entities for relationships (see type_constraints)
    // printf("lef = %ld, rig = %ld, rel = %ld", lef, rig, reference_rel);

    REAL reference_distance = probabilities[reference_head];
    INT l_s = 0;
    INT l_filter_s = 0;
    INT l_s_constrain = 0;
    INT l_filter_s_constrain = 0;

    for (INT hypothesis_head = 0; hypothesis_head < entityTotal; hypothesis_head++) {
        if (hypothesis_head != reference_head) { 
            REAL hypothesis_distance = probabilities[hypothesis_head];
            if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) { // If model thinks that the reference triple is more probable (for transe that means less distance between h + r and t)
                l_s += 1; // Count incorrectly classified triples
                if (not _find(hypothesis_head, reference_tail, reference_rel)) // If less probable triple is not present in the dataset
                    l_filter_s += 1; // Count incorrectly classified triples which are not present in the dataset (filtered score must be better than unfiltered concerning rank)
            }

            // printf("Initial head type at %ld = %ld", lef, head_type[lef]);
            while (lef < rig && head_type[lef] < hypothesis_head) lef ++; // find head entity in the list of allowed heads for current relation
            if (lef < rig && hypothesis_head == head_type[lef]) { // if current head entity is indeed allowed for the observed relation
                if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                    l_s_constrain += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads (constrained score must be better than unconstrained)
                    if (not _find(hypothesis_head, reference_tail, reference_rel)) {
                        l_filter_s_constrain += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads but triple does not exist
                    }
                }  
            }
            // else {
            //     printf("No head %ld \n", hypothesis_head);
            // }
        }
    }

    if (l_filter_s < 10) l_filter_tot += 1;
    if (l_s < 10) l_tot += 1;
    if (l_filter_s < 3) l3_filter_tot += 1;
    if (l_s < 3) l3_tot += 1;
    if (l_filter_s < 1) l1_filter_tot += 1;
    if (l_s < 1) l1_tot += 1;

    if (l_filter_s_constrain < 10) l_filter_tot_constrain += 1;
    if (l_s_constrain < 10) l_tot_constrain += 1;
    if (l_filter_s_constrain < 3) l3_filter_tot_constrain += 1;
    if (l_s_constrain < 3) l3_tot_constrain += 1;
    if (l_filter_s_constrain < 1) l1_filter_tot_constrain += 1;
    if (l_s_constrain < 1) l1_tot_constrain += 1;

    l_filter_rank += (l_filter_s+1);
    l_rank += (1+l_s);
    l_filter_reci_rank += 1.0/(l_filter_s+1);
    l_reci_rank += 1.0/(l_s+1);

    l_filter_rank_constrain += (l_filter_s_constrain+1);
    l_rank_constrain += (1+l_s_constrain);
    l_filter_reci_rank_constrain += 1.0/(l_filter_s_constrain+1);
    l_reci_rank_constrain += 1.0/(l_s_constrain+1);

    lastHead++;
}

// extern "C"
void validHead(REAL *probabilities, bool reverse) {
    INT reference_head = validList[lastHead].h;
    INT reference_tail = validList[lastHead].t;
    INT reference_rel = validList[lastHead].r;
    INT lef = head_lef[reference_rel], rig = head_rig[reference_rel]; // Left and right bound of sets of entity-ids in the common list of acceptable entities for relationships (see type_constraints)
    // printf("lef = %ld, rig = %ld, rel = %ld", lef, rig, reference_rel);

    REAL reference_distance = probabilities[reference_head];
    INT l_s = 0;
    INT l_filter_s = 0;
    INT l_s_constrain = 0;
    INT l_filter_s_constrain = 0;

    for (INT hypothesis_head = 0; hypothesis_head < entityTotal; hypothesis_head++) {
        if (hypothesis_head != reference_head) { 
            REAL hypothesis_distance = probabilities[hypothesis_head];
            if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) { // If model thinks that the reference triple is more probable (for transe that means less distance between h + r and t)
            // if (hypothesis_distance <= reference_distance) { // If model thinks that the reference triple is more probable (for transe that means less distance between h + r and t)
                l_s += 1; // Count incorrectly classified triples
                if (not _find(hypothesis_head, reference_tail, reference_rel)) // If less probable triple is not present in the dataset
                    l_filter_s += 1; // Count incorrectly classified triples which are not present in the dataset (filtered score must be better than unfiltered concerning rank)
            }

            // printf("Initial head type at %ld = %ld", lef, head_type[lef]);
            while (lef < rig && head_type[lef] < hypothesis_head) lef ++;
            if (lef < rig && hypothesis_head == head_type[lef]) {
                // if (hypothesis_distance <= reference_distance) {
                if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                    l_s_constrain += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads (constrained score must be better than unconstrained)
                    if (not _find(hypothesis_head, reference_tail, reference_rel)) {
                        l_filter_s_constrain += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads but triple does not exist
                    }
                }  
            }
            // else {
            //     printf("No head %ld \n", hypothesis_head);
            // }
        }
    }

    if (l_filter_s < 10) l_filter_tot += 1;
    if (l_s < 10) l_tot += 1;
    if (l_filter_s < 3) l3_filter_tot += 1;
    if (l_s < 3) l3_tot += 1;
    if (l_filter_s < 1) l1_filter_tot += 1;
    if (l_s < 1) l1_tot += 1;

    if (l_filter_s_constrain < 10) l_filter_tot_constrain += 1;
    if (l_s_constrain < 10) l_tot_constrain += 1;
    if (l_filter_s_constrain < 3) l3_filter_tot_constrain += 1;
    if (l_s_constrain < 3) l3_tot_constrain += 1;
    if (l_filter_s_constrain < 1) l1_filter_tot_constrain += 1;
    if (l_s_constrain < 1) l1_tot_constrain += 1;

    l_filter_rank += (l_filter_s+1);
    l_rank += (1+l_s);
    l_filter_reci_rank += 1.0/(l_filter_s+1);
    l_reci_rank += 1.0/(l_s+1);

    l_filter_rank_constrain += (l_filter_s_constrain+1);
    l_rank_constrain += (1+l_s_constrain);
    l_filter_reci_rank_constrain += 1.0/(l_filter_s_constrain+1);
    l_reci_rank_constrain += 1.0/(l_s_constrain+1);

    lastHead++;
}

// extern "C"
void testTail(REAL *probabilities, bool reverse) {
    INT h = testList[lastTail].h;
    INT t = testList[lastTail].t;
    INT r = testList[lastTail].r;
    INT lef = tail_lef[r], rig = tail_rig[r];
    REAL reference_distance = probabilities[t];
    INT r_s = 0;
    INT r_filter_s = 0;
    INT r_s_constrain = 0;
    INT r_filter_s_constrain = 0;
    for (INT j = 0; j < entityTotal; j++) {
        if (j != t) {
            REAL hypothesis_distance = probabilities[j];
            if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                // if (hypothesis_distance <= reference_distance) {
                r_s += 1;
                if (not _find(h, j, r))
                    r_filter_s += 1;
            }
            while (lef < rig && tail_type[lef] < j) lef ++;
            if (lef < rig && j == tail_type[lef]) {
                if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                    // if (hypothesis_distance <= reference_distance) {
                    r_s_constrain += 1;
                    if (not _find(h, j ,r)) {
                        r_filter_s_constrain += 1;
                    }
                }
            }
        }
        
    }

    if (r_filter_s < 10) r_filter_tot += 1;
    if (r_s < 10) r_tot += 1;
    if (r_filter_s < 3) r3_filter_tot += 1;
    if (r_s < 3) r3_tot += 1;
    if (r_filter_s < 1) r1_filter_tot += 1;
    if (r_s < 1) r1_tot += 1;

    if (r_filter_s_constrain < 10) r_filter_tot_constrain += 1;
    if (r_s_constrain < 10) r_tot_constrain += 1;
    if (r_filter_s_constrain < 3) r3_filter_tot_constrain += 1;
    if (r_s_constrain < 3) r3_tot_constrain += 1;
    if (r_filter_s_constrain < 1) r1_filter_tot_constrain += 1;
    if (r_s_constrain < 1) r1_tot_constrain += 1;

    r_filter_rank += (1+r_filter_s);
    r_rank += (1+r_s);
    r_filter_reci_rank += 1.0/(1+r_filter_s);
    r_reci_rank += 1.0/(1+r_s);

    r_filter_rank_constrain += (1+r_filter_s_constrain);
    r_rank_constrain += (1+r_s_constrain);
    r_filter_reci_rank_constrain += 1.0/(1+r_filter_s_constrain);
    r_reci_rank_constrain += 1.0/(1+r_s_constrain);

    lastTail++;
}

// extern "C"
void validTail(REAL *probabilities, bool reverse) {
    INT h = validList[lastTail].h;
    INT t = validList[lastTail].t;
    INT r = validList[lastTail].r;
    INT lef = tail_lef[r], rig = tail_rig[r];
    REAL reference_distance = probabilities[t];
    INT r_s = 0;
    INT r_filter_s = 0;
    INT r_s_constrain = 0;
    INT r_filter_s_constrain = 0;
    for (INT j = 0; j < entityTotal; j++) {
        if (j != t) {
            REAL hypothesis_distance = probabilities[j];
            // if (hypothesis_distance < reference_distance) {
            if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                r_s += 1;
                if (not _find(h, j, r))
                    r_filter_s += 1;
            }
            while (lef < rig && tail_type[lef] < j) lef ++;
            if (lef < rig && j == tail_type[lef]) {
                if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                    // if (hypothesis_distance < reference_distance) {
                    r_s_constrain += 1;
                    if (not _find(h, j ,r)) {
                        r_filter_s_constrain += 1;
                    }
                }
            }
        }
        
    }

    if (r_filter_s < 10) r_filter_tot += 1;
    if (r_s < 10) r_tot += 1;
    if (r_filter_s < 3) r3_filter_tot += 1;
    if (r_s < 3) r3_tot += 1;
    if (r_filter_s < 1) r1_filter_tot += 1;
    if (r_s < 1) r1_tot += 1;

    if (r_filter_s_constrain < 10) r_filter_tot_constrain += 1;
    if (r_s_constrain < 10) r_tot_constrain += 1;
    if (r_filter_s_constrain < 3) r3_filter_tot_constrain += 1;
    if (r_s_constrain < 3) r3_tot_constrain += 1;
    if (r_filter_s_constrain < 1) r1_filter_tot_constrain += 1;
    if (r_s_constrain < 1) r1_tot_constrain += 1;

    r_filter_rank += (1+r_filter_s);
    r_rank += (1+r_s);
    r_filter_reci_rank += 1.0/(1+r_filter_s);
    r_reci_rank += 1.0/(1+r_s);

    r_filter_rank_constrain += (1+r_filter_s_constrain);
    r_rank_constrain += (1+r_s_constrain);
    r_filter_reci_rank_constrain += 1.0/(1+r_filter_s_constrain);
    r_reci_rank_constrain += 1.0/(1+r_s_constrain);

    lastTail++;
}

// extern "C"
void test_link_prediction(bool as_tsv = false) {
    // printf("As tsv = %d\n", as_tsv);
    // l_* - metrics computed on batches with replacing triple heads with all possible entities
    // r_* - metrics computed on batches with replacing triple tails with all possible entities

    l_rank /= testTotal;
    r_rank /= testTotal;
    l_reci_rank /= testTotal;
    r_reci_rank /= testTotal;
 
    l_tot /= testTotal;
    l3_tot /= testTotal;
    l1_tot /= testTotal;
 
    r_tot /= testTotal;
    r3_tot /= testTotal;
    r1_tot /= testTotal;

    // with filter
    l_filter_rank /= testTotal;
    r_filter_rank /= testTotal;
    l_filter_reci_rank /= testTotal;
    r_filter_reci_rank /= testTotal;
 
    l_filter_tot /= testTotal;
    l3_filter_tot /= testTotal;
    l1_filter_tot /= testTotal;
 
    r_filter_tot /= testTotal;
    r3_filter_tot /= testTotal;
    r1_filter_tot /= testTotal;

    if (!as_tsv) {
        printf("no type constraint results:\n");
    }
    
    printf("metric:\tMRR\tMR\thit@10\thit@3\thit@1\n");
    printf("head (all entities):\t%f\t%f\t%f\t%f\t%f\n", l_reci_rank, l_rank, l_tot, l3_tot, l1_tot);
    printf("tail (all entities):\t%f\t%f\t%f\t%f\t%f\n", r_reci_rank, r_rank, r_tot, r3_tot, r1_tot);
    printf("averaged (all entities):\t%f\t%f\t%f\t%f\t%f\n",
            (l_reci_rank+r_reci_rank)/2, (l_rank+r_rank)/2, (l_tot+r_tot)/2, (l3_tot+r3_tot)/2, (l1_tot+r1_tot)/2);
    if (!as_tsv) {
        printf("\n");
    }
    printf("head (triples not present in the dataset):\t%f\t%f\t%f\t%f\t%f\n", l_filter_reci_rank, l_filter_rank, l_filter_tot, l3_filter_tot, l1_filter_tot);
    printf("tail (triples not present in the dataset):\t%f\t%f\t%f\t%f\t%f\n", r_filter_reci_rank, r_filter_rank, r_filter_tot, r3_filter_tot, r1_filter_tot);
    printf("averaged (triples not present in the dataset):\t%f\t%f\t%f\t%f\t%f\n",
            (l_filter_reci_rank+r_filter_reci_rank)/2, (l_filter_rank+r_filter_rank)/2, (l_filter_tot+r_filter_tot)/2, (l3_filter_tot+r3_filter_tot)/2, (l1_filter_tot+r1_filter_tot)/2);

    //type constrain
    l_rank_constrain /= testTotal;
    r_rank_constrain /= testTotal;
    l_reci_rank_constrain /= testTotal;
    r_reci_rank_constrain /= testTotal;
 
    l_tot_constrain /= testTotal;
    l3_tot_constrain /= testTotal;
    l1_tot_constrain /= testTotal;
 
    r_tot_constrain /= testTotal;
    r3_tot_constrain /= testTotal;
    r1_tot_constrain /= testTotal;

    // with filter
    l_filter_rank_constrain /= testTotal;
    r_filter_rank_constrain /= testTotal;
    l_filter_reci_rank_constrain /= testTotal;
    r_filter_reci_rank_constrain /= testTotal;
 
    l_filter_tot_constrain /= testTotal;
    l3_filter_tot_constrain /= testTotal;
    l1_filter_tot_constrain /= testTotal;
 
    r_filter_tot_constrain /= testTotal;
    r3_filter_tot_constrain /= testTotal;
    r1_filter_tot_constrain /= testTotal;

    if (!as_tsv) {
        printf("\n");
        printf("\n");
        printf("type constraint results:\n");
        printf("\n");
        printf("\n");
    }
    
    printf("metric:\tMRR\tMR\thit@10\thit@3\thit@1\n");
    printf("head (all entities):\t%f\t%f\t%f\t%f\t%f\n", l_reci_rank_constrain, l_rank_constrain, l_tot_constrain, l3_tot_constrain, l1_tot_constrain);
    printf("tail (all entities):\t%f\t%f\t%f\t%f\t%f\n", r_reci_rank_constrain, r_rank_constrain, r_tot_constrain, r3_tot_constrain, r1_tot_constrain);
    printf("averaged (all entities):\t%f\t%f\t%f\t%f\t%f\n",
            (l_reci_rank_constrain+r_reci_rank_constrain)/2, (l_rank_constrain+r_rank_constrain)/2, (l_tot_constrain+r_tot_constrain)/2, (l3_tot_constrain+r3_tot_constrain)/2, (l1_tot_constrain+r1_tot_constrain)/2);
    if (!as_tsv) {
        printf("\n");
    }
    printf("head (triples not present in the dataset):\t%f\t%f\t%f\t%f\t%f\n", l_filter_reci_rank_constrain, l_filter_rank_constrain, l_filter_tot_constrain, l3_filter_tot_constrain, l1_filter_tot_constrain);
    printf("tail (triples not present in the dataset):\t%f\t%f\t%f\t%f\t%f\n", r_filter_reci_rank_constrain, r_filter_rank_constrain, r_filter_tot_constrain, r3_filter_tot_constrain, r1_filter_tot_constrain);
    printf("averaged (triples not present in the dataset):\t%f\t%f\t%f\t%f\t%f\n", (l_filter_reci_rank_constrain+r_filter_reci_rank_constrain)/2, (l_filter_rank_constrain+r_filter_rank_constrain)/2, (l_filter_tot_constrain+r_filter_tot_constrain)/2, (l3_filter_tot_constrain+r3_filter_tot_constrain)/2, (l1_filter_tot_constrain+r1_filter_tot_constrain)/2);
}

/*=====================================================================================
triple classification
======================================================================================*/
Triple *negTestList;
extern "C"
void getNegTest() {
    negTestList = (Triple *)calloc(testTotal, sizeof(Triple));
    for (INT i = 0; i < testTotal; i++) {
        negTestList[i] = testList[i];
        negTestList[i].t = corrupt(testList[i].h, testList[i].r);
    }
}

Triple *negValidList;
extern "C"
void getNegValid() {
    negValidList = (Triple *)calloc(validTotal, sizeof(Triple));
    for (INT i = 0; i < validTotal; i++) {
        negValidList[i] = validList[i];
        negValidList[i].t = corrupt(validList[i].h, validList[i].r);
    }   
}

extern // "C"
void getTestBatch(INT *ph, INT *pt, INT *pr, INT *nh, INT *nt, INT *nr) {
    getNegTest();
    for (INT i = 0; i < testTotal; i++) {
        ph[i] = testList[i].h;
        pt[i] = testList[i].t;
        pr[i] = testList[i].r;
        nh[i] = negTestList[i].h;
        nt[i] = negTestList[i].t;
        nr[i] = negTestList[i].r;
    }
}

extern // "C"
void getValidBatch(INT *ph, INT *pt, INT *pr, INT *nh, INT *nt, INT *nr) {
    getNegValid();
    for (INT i = 0; i < validTotal; i++) {
        ph[i] = validList[i].h;
        pt[i] = validList[i].t;
        pr[i] = validList[i].r;
        nh[i] = negValidList[i].h;
        nt[i] = negValidList[i].t;
        nr[i] = negValidList[i].r;
    }
}

REAL threshEntire;
extern // "C"
void getBestThreshold(REAL *relThresh, REAL *score_pos, REAL *score_neg) {
    REAL interval = 0.01;
    REAL min_score, max_score, bestThresh, tmpThresh, bestAcc, tmpAcc;
    INT n_interval, correct, total;
    for (INT r = 0; r < relationTotal; r++) {
        if (validLef[r] == -1) continue;
        total = (validRig[r] - validLef[r] + 1) * 2;
        min_score = score_pos[validLef[r]];
        if (score_neg[validLef[r]] < min_score) min_score = score_neg[validLef[r]];
        max_score = score_pos[validLef[r]];
        if (score_neg[validLef[r]] > max_score) max_score = score_neg[validLef[r]];
        for (INT i = validLef[r]+1; i <= validRig[r]; i++) {
            if(score_pos[i] < min_score) min_score = score_pos[i];
            if(score_pos[i] > max_score) max_score = score_pos[i];
            if(score_neg[i] < min_score) min_score = score_neg[i];
            if(score_neg[i] > max_score) max_score = score_neg[i];
        }
        n_interval = INT((max_score - min_score)/interval);
        for (INT i = 0; i <= n_interval; i++) {
            tmpThresh = min_score + i * interval;
            correct = 0;
            for (INT j = validLef[r]; j <= validRig[r]; j++) {
                if (score_pos[j] <= tmpThresh) correct ++;
                if (score_neg[j] > tmpThresh) correct ++;
            }
            tmpAcc = 1.0 * correct / total;
            if (i == 0) {
                bestThresh = tmpThresh;
                bestAcc = tmpAcc;
            } else if (tmpAcc > bestAcc) {
                bestAcc = tmpAcc;
                bestThresh = tmpThresh;
            }
        }
        relThresh[r] = bestThresh;
    }
}

REAL *testAcc;
REAL aveAcc;
extern // "C"
void test_triple_classification(REAL *relThresh, REAL *score_pos, REAL *score_neg) {
    testAcc = (REAL *)calloc(relationTotal, sizeof(REAL));
    INT aveCorrect = 0, aveTotal = 0;
    REAL aveAcc;
    for (INT r = 0; r < relationTotal; r++) {
        if (validLef[r] == -1 || testLef[r] ==-1) continue;
        INT correct = 0, total = 0;
        for (INT i = testLef[r]; i <= testRig[r]; i++) {
            if (score_pos[i] <= relThresh[r]) correct++;
            if (score_neg[i] > relThresh[r]) correct++;
            total += 2;
        }
        testAcc[r] = 1.0 * correct / total;
        aveCorrect += correct; 
        aveTotal += total;
    }
    aveAcc = 1.0 * aveCorrect / aveTotal;
    printf("triple classification accuracy is %lf\n", aveAcc);
}

