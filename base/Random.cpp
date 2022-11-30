#include <cstdlib>

#include "types.h"
// #include "../../randeer/randeer/state/RandomizationState.h"
#include "state/RandomizationState.h"
#include "state/JavaLikeLcgRandomizationState.h"

using namespace std;

RandomizationState** randomizationStates;
// unsigned long long* next_random;

long k = 0;

extern "C"
void randReset(INT nWorkers) { // Generate random numbers for each thread
	// next_random = (unsigned long long *)calloc(nWorkers, sizeof(unsigned long long));
	// randomizationStates = (RandomizationState**)calloc(nWorkers, sizeof(RandomizationState*));
	// for (INT i = 0; i < nWorkers; i++) {
	// 	// next_random[i] = rand();
	// 	// randomizationStates[i] = new JavaLikeLcgRandomizationState(rand());
	// 	randomizationStates[i] = new JavaLikeLcgRandomizationState(18);
    // }
}

unsigned long long randd(INT id) { // Generate sequential random numbers
    // next_random[id] = next_random[id] * (unsigned long long)25214903917 + 11;
    // return next_random[id];
    return randomizationStates[id]->sample();
}

INT rand_max(INT id, INT x) {
	INT res = randd(id) % x;
	while (res < 0)
		res += x;
	return res;
}

INT rand_max(RandomizationState* randomizer, INT x) {
    unsigned long long sampledNumber = randomizer->sample();
	INT res = sampledNumber % x;
	// INT res = randd(id) % x;
	while (res < 0)
		res += x;
	return res;
}

INT rand(INT a, INT b){ // Sample a random integer in the interval [a, b)
	return (rand() % (b-a))+ a;
}

