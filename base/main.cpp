#include "Setting.h"
#include "Random.h"
#include "Reader.h"
#include "Corrupt.h"
#include "Test.h"
#include "Valid.h"
#include <cstdlib>
#include <pthread.h>
#include "main.h"

#include "patterns/main.h"

// #include "patterns/symmetric/main.h"
#include "patterns/symmetric/reader.h"

// #include "patterns/none/main.h"
#include "patterns/none/reader.h"


// const unsigned int nPatternInstanceSets = 3;
extern // "C"
// template<unsigned int nPatternInstanceSets>
void sampling(INT *batch_h, INT *batch_t, INT *batch_r, REAL *batch_y, INT batchSize, INT negRate, INT negRelRate, INT headBatchFlag, string pattern_name, int nObservedTriplesPerPatternInstance) {
    // const unsigned int nPatternInstanceSets = 3;
    // std::cout << "Started sampling in " << workThreads << " threads" << endl;
	pthread_t *pt = (pthread_t *)malloc(workThreads * sizeof(pthread_t));
	Parameter *para = (Parameter *)malloc(workThreads * sizeof(Parameter));

    // cout << "N observed triples per pattern instance = " << nObservedTriplesPerPatternInstance << endl;

    for (INT thread_index = 0; thread_index < workThreads; thread_index++) {
        para[thread_index].id = thread_index;
        para[thread_index].batch_h = batch_h;
        para[thread_index].batch_t = batch_t;
        para[thread_index].batch_r = batch_r;
        para[thread_index].batch_y = batch_y;
        para[thread_index].batchSize = batchSize;
        para[thread_index].negRate = negRate;
        para[thread_index].negRelRate = negRelRate;
        para[thread_index].headBatchFlag = headBatchFlag;
        para[thread_index].nObservedTriplesPerPatternInstance = nObservedTriplesPerPatternInstance;
        // std::cout << "Creating thread...\n\r";
        // switch(pattern) {
        //     case none:
        //         // pthread_create(&pt[thread_index], NULL, getBatch, (void*)(para+thread_index));
        //         para[thread_index].patternInstanceSets = noneTriplePatternInstances; // (std::vector<PatternInstance>**)
        //         pthread_create(&pt[thread_index], NULL, getPatternBatch, (void*)(para+thread_index));
        //         break;
        //     case symmetric:
        //         para[thread_index].patternInstanceSets = symmetricTriplePatternInstances; // (std::vector<PatternInstance>**)
        //         pthread_create(&pt[thread_index], NULL, getPatternBatch, (void*)(para+thread_index));
        //         break;
        // }
        // cout << "instance sets + " << pattern_name << " " << patternDescriptions[pattern_name].id << " " << patternDescriptions[pattern_name].instanceSets << " " << noneTriplePatternInstances << endl;
        para[thread_index].patternInstanceSets = patternDescriptions[decodePatternName(pattern_name)].instanceSets; // (std::vector<PatternInstance>**)
        // cout << "instance sets-" << endl;
        pthread_create(&pt[thread_index], NULL, getPatternBatch, (void*)(para+thread_index));
    }
    for (INT thread_index = 0; thread_index < workThreads; thread_index++) {
        pthread_join(pt[thread_index], NULL);
        // cout << "JOINED" << endl;
    }
    // free(pt);
    // free(para);
}

// extern // "C"
// void sampling(INT *batch_h, INT *batch_t, INT *batch_r, REAL *batch_y, INT batchSize, INT negRate, INT negRelRate, INT headBatchFlag) {
//     sampling(batch_h, batch_t, batch_r, batch_y, batchSize, negRate, negRelRate, headBatchFlag, nonePatternName, 0);
// }

int main() {
	importTrainFiles();
	return 0;
}

