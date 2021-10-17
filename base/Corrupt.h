#ifndef CORRUPT_H
#define CORRUPT_H
#include "Random.h"
#include "Triple.h"
#include "Reader.h"

INT corrupt_head(INT id, INT h, INT r) { // Sample an incorrect tail given head, relationship and thread id (thread id is used for randomization)
	INT lef, rig, mid, ll, rr;

	lef = lefHead[h] - 1;
	rig = rigHead[h];
	while (lef + 1 < rig) { // Find location of the first triple having required relationship and save it to the rig (and subsequently - to the ll) variable
		mid = (lef + rig) >> 1;
		if (trainHead[mid].r >= r) rig = mid; else // If same relationship then move right boundary closer to the left (i.e. closer to the beginning of the list)
		lef = mid;
	}
	ll = rig;

	lef = lefHead[h];
	rig = rigHead[h] + 1;
	while (lef + 1 < rig) { // Find location of the last triple having required relationship and save it to the lef (and subsequently - to the rr) variable
		mid = (lef + rig) >> 1;
		if (trainHead[mid].r <= r) lef = mid; else // If same relationship then move left boundary closer to the right (i.e. closer to the ending of the list)
		rig = mid;
	}
	rr = lef;

	INT tmp = rand_max(id, entityTotal - (rr - ll + 2)); // Generate random entity index in the interval [0; nEntities - (nTailEntitiesForGivenHead + nHeadEntitiesForGivenHead)]
	if (tmp < trainHead[ll].t) return tmp; // If generated entity index is less than any other tail entity index (in other case the generated triple would probably not be unique) then return this
	// if (tmp + rr - ll + 1 > trainHead[rr].t) return tmp + rr - ll + 1;
	if (tmp > trainHead[rr].t - rr + ll - 1) return tmp + rr - ll + 1; // If generated entity index + max possible offset is larger than any other tail entity index then return this

	lef = ll, rig = rr + 1;
	while (lef + 1 < rig) { // While the left and right boundaries do not overlap (they differ at least by one)
		mid = (lef + rig) >> 1;
		// if (trainHead[mid].t + (ll - mid) - 1 < tmp)
		// if (trainHead[mid].t < tmp + (mid - ll) + 1)
		if (trainHead[mid].t - mid + ll - 1 < tmp)
			lef = mid;
		else 
			rig = mid;
	} // Find an optimal value of tmp + offset which does not overlap with present triples
	return tmp + lef - ll + 1;
}

INT corrupt_tail(INT id, INT t, INT r) { // Sample an incorrect head given tail, relationship and thread id (thread id is used for randomization)
	INT lef, rig, mid, ll, rr;
	lef = lefTail[t] - 1;
	rig = rigTail[t];
	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainTail[mid].r >= r) rig = mid; else
		lef = mid;
	}
	ll = rig;
	lef = lefTail[t];
	rig = rigTail[t] + 1;
	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainTail[mid].r <= r) lef = mid; else
		rig = mid;
	}
	rr = lef;
	INT tmp = rand_max(id, entityTotal - (rr - ll + 1));
	if (tmp < trainTail[ll].h) return tmp;
	if (tmp > trainTail[rr].h - rr + ll - 1) return tmp + rr - ll + 1;
	lef = ll, rig = rr + 1;
	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainTail[mid].h - mid + ll - 1 < tmp)
			lef = mid;
		else 
			rig = mid;
	}
	return tmp + lef - ll + 1;
}


INT corrupt_rel(INT id, INT h, INT t) { // Sample an incorrect relationship given head and tail
	INT lef, rig, mid, ll, rr;
	lef = lefRel[h] - 1;
	rig = rigRel[h];
	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainRel[mid].t >= t) rig = mid; else
		lef = mid;
	}
	ll = rig;
	lef = lefRel[h];
	rig = rigRel[h] + 1;
	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainRel[mid].t <= t) lef = mid; else
		rig = mid;
	}
	rr = lef;
	INT tmp = rand_max(id, relationTotal - (rr - ll + 1));
	if (tmp < trainRel[ll].r) return tmp;
	if (tmp > trainRel[rr].r - rr + ll - 1) return tmp + rr - ll + 1;
	lef = ll, rig = rr + 1;
	while (lef + 1 < rig) {
		mid = (lef + rig) >> 1;
		if (trainRel[mid].r - mid + ll - 1 < tmp)
			lef = mid;
		else 
			rig = mid;
	}
	return tmp + lef - ll + 1;
}


bool _find(INT h, INT t, INT r) { // Check whether a triple is presented in the dataset (tripleList variables contains triples from all subsets)
    INT lef = 0;
    INT rig = tripleTotal - 1;
    INT mid;
    while (lef + 1 < rig) {
        INT mid = (lef + rig) >> 1;
        if ((tripleList[mid]. h < h) || (tripleList[mid]. h == h && tripleList[mid]. r < r) || (tripleList[mid]. h == h && tripleList[mid]. r == r && tripleList[mid]. t < t)) lef = mid; else rig = mid;
    }
    if (tripleList[lef].h == h && tripleList[lef].r == r && tripleList[lef].t == t) return true;
    if (tripleList[rig].h == h && tripleList[rig].r == r && tripleList[rig].t == t) return true;
    return false;
}

INT corrupt(INT h, INT r){
	INT ll = tail_lef[r];
	INT rr = tail_rig[r];
	INT loop = 0;
	INT t;
	while(1) {
		t = tail_type[rand(ll, rr)]; // Select random tail entity id for a given relation according to type mappings
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
#endif
