#include "types.h"
#include <cstdlib>

unsigned long long *next_random;

extern "C"
void randReset(INT nWorkers) { // Generate random numbers for each thread
	next_random = (unsigned long long *)calloc(nWorkers, sizeof(unsigned long long));
	for (INT i = 0; i < nWorkers; i++)
		next_random[i] = rand();
}

unsigned long long randd(INT id) { // Generate sequential random numbers
	next_random[id] = next_random[id] * (unsigned long long)25214903917 + 11;
	return next_random[id];
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

