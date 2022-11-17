#include "Random.h"
#include "triple/main.h"
#include "Reader.h"

INT corrupt_head(INT id, INT h, INT r) { // Sample an incorrect tail given head, relationship and thread id (thread id is used for randomization)
	INT lef, rig, mid, ll, rr;

	lef = trainLists->head->left[h] - 1;
	rig = trainLists->head->right[h];
	while (lef + 1 < rig) { // Find location of the first triple having required relationship and save it to the rig (and subsequently - to the ll) variable
		mid = (lef + rig) >> 1;
		if (trainLists->head->items[mid].r >= r) rig = mid; else // If same relationship then move right boundary closer to the left (i.e. closer to the beginning of the list)
		lef = mid;
	}
	ll = rig;

	lef = trainLists->head->left[h];
	rig = trainLists->head->right[h] + 1;
	while (lef + 1 < rig) { // Find location of the last triple having required relationship and save it to the lef (and subsequently - to the rr) variable
		mid = (lef + rig) >> 1;
		if (trainLists->head->items[mid].r <= r) lef = mid; else // If same relationship then move left boundary closer to the right (i.e. closer to the ending of the list)
		rig = mid;
	}
	rr = lef;

	INT tmp = rand_max(id, trainLists->frequencies->nEntities - (rr - ll + 2)); // Generate random entity index in the interval [0; nEntities - (nTailEntitiesForGivenHead + nHeadEntitiesForGivenHead)]
	if (tmp < trainLists->head->items[ll].t) return tmp; // If generated entity index is less than any other tail entity index (in other case the generated triple would probably not be unique) then return this
	// if (tmp + rr - ll + 1 > trainHead[rr].t) return tmp + rr - ll + 1;
	if (tmp > trainLists->head->items[rr].t - rr + ll - 1) return tmp + rr - ll + 1; // If generated entity index + max possible offset is larger than any other tail entity index then return this

	lef = ll, rig = rr + 1;
	while (lef + 1 < rig) { // While the left and right boundaries do not overlap (they differ at least by one)
		mid = (lef + rig) >> 1;
		// if (trainHead[mid].t + (ll - mid) - 1 < tmp)
		// if (trainHead[mid].t < tmp + (mid - ll) + 1)
		if (trainLists->head->items[mid].t - mid + ll - 1 < tmp)
			lef = mid;
		else 
			rig = mid;
	} // Find an optimal value of tmp + offset which does not overlap with present triples
	return tmp + lef - ll + 1;
}

INT corrupt_tail(INT id, INT t, INT r) { // Sample an incorrect head given tail, relationship and thread id (thread id is used for randomization)
	INT lef, rig, mid, ll, rr;
	lef = trainLists->tail->left[t] - 1;
	rig = trainLists->tail->right[t];
	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainLists->tail->items[mid].r >= r) rig = mid; else
		lef = mid;
	}
	ll = rig;

	lef = trainLists->tail->left[t];
	rig = trainLists->tail->right[t] + 1;
	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainLists->tail->items[mid].r <= r) lef = mid; else
		rig = mid;
	}
	rr = lef;

	INT tmp = rand_max(id, trainLists->frequencies->nEntities - (rr - ll + 1));
	if (tmp < trainLists->tail->items[ll].h) return tmp;
	if (tmp > trainLists->tail->items[rr].h - rr + ll - 1) return tmp + rr - ll + 1;
	lef = ll, rig = rr + 1;
	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainLists->tail->items[mid].h - mid + ll - 1 < tmp)
			lef = mid;
		else 
			rig = mid;
	}
	return tmp + lef - ll + 1;
}


INT corrupt_rel(INT id, INT h, INT t) { // Sample an incorrect relationship given head and tail
	INT lef, rig, mid, ll, rr;
	lef = trainLists->relation->left[h] - 1;
	rig = trainLists->relation->right[h];
	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainLists->relation->items[mid].t >= t) rig = mid; else
		lef = mid;
	}
	ll = rig;

	lef = trainLists->relation->left[h];
	rig = trainLists->relation->right[h] + 1;

	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainLists->relation->items[mid].t <= t) lef = mid; else
		rig = mid;
	}
	rr = lef;
	INT tmp = rand_max(id, trainLists->frequencies->nRelations - (rr - ll + 1));
	if (tmp < trainLists->relation->items[ll].r) return tmp;
	if (tmp > trainLists->relation->items[rr].r - rr + ll - 1) return tmp + rr - ll + 1;
	lef = ll, rig = rr + 1;
	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainLists->relation->items[mid].r - mid + ll - 1 < tmp)
			lef = mid;
		else 
			rig = mid;
	}
	return tmp + lef - ll + 1;
}


bool _find(INT h, INT t, INT r) { // Check whether a triple is presented in the dataset (tripleList variables contains triples from all subsets)
    Triple triple = Triple(h, r, t);

    return trainLists->index->contains(triple) || testLists->index->contains(triple) || validLists->index->contains(triple);

    // INT lef = 0;
    // INT rig = tripleTotal - 1;
    // INT mid;
    // while (lef + 1 < rig) {
    //     INT mid = (lef + rig) >> 1;
    //     if ((tripleList[mid]. h < h) || (tripleList[mid]. h == h && tripleList[mid]. r < r) || (tripleList[mid]. h == h && tripleList[mid]. r == r && tripleList[mid]. t < t)) lef = mid; else rig = mid;
    // }
    // if (tripleList[lef].h == h && tripleList[lef].r == r && tripleList[lef].t == t) return true;
    // if (tripleList[rig].h == h && tripleList[rig].r == r && tripleList[rig].t == t) return true;
    // return false;
}

INT corrupt(INT h, INT r){
	// INT ll = tail_lef[r];
	// INT rr = tail_rig[r];
	INT loop = 0;
	INT t;
	while(1) {
		t = types->get(r)->tails->items[rand(0, types->get(r)->tails->length)]; // Select random tail entity id for a given relation according to type mappings
		if (not _find(h, t, r)) { // If obtained triple does not exist, then it is a suitable negative sample which may be immediately returned
		//	printf("r:%ld\tt:%ld\n", r, t);
			return t;
		} else { // If not, then repeat the stochastic approach 1000 times and if it still doesn't work, then try to apply a more precise algorithm
			loop ++;
			if (loop >= 1000){
			//	printf("drop\n");
				return corrupt_head(0, h, r);
			}
		} 
	}
}

