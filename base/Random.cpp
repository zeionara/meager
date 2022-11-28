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
	randomizationStates = (RandomizationState**)calloc(nWorkers, sizeof(RandomizationState*));
	for (INT i = 0; i < nWorkers; i++) {
		// next_random[i] = rand();
        //
        // long seed = rand();
        // cout << "Init randomizer" << endl;
		// randomizationStates[i] = new JavaLikeLcgRandomizationState(seed);
		randomizationStates[i] = new JavaLikeLcgRandomizationState(rand());
        // cout << randomizationStates[i]->state << endl;
        // randomizationStates[i]->reset(randa + 1);
        // cout << randomizationStates[i]->multiplier * randomizationStates[i]->state << endl;
        // cout << randomizationStates[i]->sample() << endl;
		// next_random[i] = seed;

        // cout << "vv" << endl;
        // cout << randomizationStates[i]->state << endl;
        // cout << next_random[i] << endl;
        // cout << "^^" << endl;
    }
}

unsigned long long randd(INT id) { // Generate sequential random numbers
    // next_random[id] = next_random[id] * (unsigned long long)25214903917 + 11;
    // return next_random[id];
    //
    // unsigned long long value = next_random[id];
	// long value = randomizationStates[id]->sample();
    // std::cout << value << std::endl;
    // unsigned long long sampledValue = randomizationStates[id]->sample();
    // if (k < 5) {
    //     cout << "--" << endl;
    //     cout << sampledValue << endl;
    //     cout << value << endl;
    //     cout << "==" << endl;
    // }
    // k += 1;
    // return value;
    // return sampledValue;
    return randomizationStates[id]->sample();
}

INT rand_max(INT id, INT x) {
	INT res = randd(id) % x;
	while (res < 0)
		res += x;
	return res;
}

INT rand(INT a, INT b){ // Sample a random integer in the interval [a, b)
	return (rand() % (b-a))+ a;
}

