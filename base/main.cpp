#include "Setting.h"
#include "Random.h"
#include "Reader.h"
#include "Corrupt.h"
#include "Test.h"
#include "Valid.h"
#include <cstdlib>
#include <pthread.h>
#include "main.h"

#include "patterns/symmetric/main.h"

extern "C"
void setInPath(char *path, bool as_tsv);

extern "C"
void setOutPath(char *path);

extern "C"
void setWorkThreads(INT threads);

extern "C"
void setBern(INT con);

extern "C"
void setHeadTailCrossSampling(INT con);

extern "C"
bool judgeHeadBatch();

extern "C"
INT getWorkThreads();

extern "C"
INT getEntityTotal();

extern "C"
INT getRelationTotal();

extern "C"
INT getTripleTotal();

extern "C"
INT getTrainTotal();

extern "C"
INT getTestTotal();

extern "C"
INT getValidTotal();

extern "C"
void randReset();

extern "C"
void importTrainFiles(bool);

void* getBatch(void* con) {
	Parameter *para = (Parameter *)(con);
	INT thread_index = para -> id;
	INT *batch_h = para -> batch_h;
	INT *batch_t = para -> batch_t;
	INT *batch_r = para -> batch_r;
	REAL *batch_y = para -> batch_y;
	INT batchSize = para -> batchSize;
	INT n_negative_triples_per_positive = para -> negRate;
	INT n_negative_triples_with_corrupted_relation_per_positive = para -> negRelRate;
	INT headBatchFlag = para -> headBatchFlag;
	INT first_triple_index, last_triple_index;

    // printf("Number of symmetric triples = %d\n", nSymmetricTriples);

    // Get interval of triples which must be initialized in this thread [first_triple_index; last_triple_index)
	if (batchSize % workThreads == 0) {
		first_triple_index = thread_index * (batchSize / workThreads);
		last_triple_index = (thread_index + 1) * (batchSize / workThreads);
	} else {
		first_triple_index = thread_index * (batchSize / workThreads + 1); // round up?
		last_triple_index = (thread_index + 1) * (batchSize / workThreads + 1);
		if (last_triple_index > batchSize) last_triple_index = batchSize;
	}
	REAL head_corruption_threshold = 500;
	for (INT current_triple_index = first_triple_index; current_triple_index < last_triple_index; current_triple_index++) {
        // Sample a positive triple randomly
		INT sampled_triple_index = rand_max(thread_index, trainTotal);
		batch_h[current_triple_index] = trainList[sampled_triple_index].h;
		batch_t[current_triple_index] = trainList[sampled_triple_index].t;
		batch_r[current_triple_index] = trainList[sampled_triple_index].r;
		batch_y[current_triple_index] = 1;
		INT last = batchSize;
        // Sample negative triples
		for (INT negative_triple_index = 0; negative_triple_index < n_negative_triples_per_positive; negative_triple_index++) {
			if (!crossSamplingFlag){
				if (bernFlag) // flag for considering a portion of triples with unique head/tail for those of which there is a given relationship
					head_corruption_threshold = 1000 * right_mean[trainList[sampled_triple_index].r] / (right_mean[trainList[sampled_triple_index].r] + left_mean[trainList[sampled_triple_index].r]);
				if (randd(thread_index) % 1000 < head_corruption_threshold) { // Corrupt TAIL by generating a random number
					batch_h[current_triple_index + last] = trainList[sampled_triple_index].h;
					batch_t[current_triple_index + last] = corrupt_head(thread_index, trainList[sampled_triple_index].h, trainList[sampled_triple_index].r);
					batch_r[current_triple_index + last] = trainList[sampled_triple_index].r;
				} else { // Corrupt HEAD
					batch_h[current_triple_index + last] = corrupt_tail(thread_index, trainList[sampled_triple_index].t, trainList[sampled_triple_index].r);
					batch_t[current_triple_index + last] = trainList[sampled_triple_index].t;
					batch_r[current_triple_index + last] = trainList[sampled_triple_index].r;
				}
				batch_y[current_triple_index + last] = -1;
				last += batchSize; // There will be <batchSize> triples generated by other threads which must be skipped
			} else  {
				if(headBatchFlag){ // Corrupt HEAD by using provided flag which specifies which part of triple must be corrupted
					batch_h[current_triple_index + last] = corrupt_tail(thread_index, trainList[sampled_triple_index].t, trainList[sampled_triple_index].r);
					batch_t[current_triple_index + last] = trainList[sampled_triple_index].t;
					batch_r[current_triple_index + last] = trainList[sampled_triple_index].r;
				} else { // Corrupt TAIL
					batch_h[current_triple_index + last] = trainList[sampled_triple_index].h;
					batch_t[current_triple_index + last] = corrupt_head(thread_index, trainList[sampled_triple_index].h, trainList[sampled_triple_index].r);
					batch_r[current_triple_index + last] = trainList[sampled_triple_index].r;
				}
				batch_y[current_triple_index + last] = -1;
				last += batchSize;
			}
		} // end loop for each negative triple with corrupted entity
        // Corrupt relations
		for (INT negative_triple_index = 0; negative_triple_index < n_negative_triples_with_corrupted_relation_per_positive; negative_triple_index++) {
			batch_h[current_triple_index + last] = trainList[sampled_triple_index].h;
			batch_t[current_triple_index + last] = trainList[sampled_triple_index].t;
			batch_r[current_triple_index + last] = corrupt_rel(thread_index, trainList[sampled_triple_index].h, trainList[sampled_triple_index].t);
			batch_y[current_triple_index + last] = -1;
			last += batchSize;
		}
	} // end loop for each positive triple in the batch which should be processed by the thread
	pthread_exit(NULL);
}


extern // "C"
void sampling(INT *batch_h, INT *batch_t, INT *batch_r, REAL *batch_y, INT batchSize, INT negRate, INT negRelRate, INT headBatchFlag, Pattern pattern, int nObservedTriplesPerPatternInstance) {
    // std::cout << "Started sampling";
	pthread_t *pt = (pthread_t *)malloc(workThreads * sizeof(pthread_t));
	Parameter *para = (Parameter *)malloc(workThreads * sizeof(Parameter));

    cout << "N observed triples per pattern instance = " << nObservedTriplesPerPatternInstance << endl;

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
        switch(pattern) {
            case none:
                pthread_create(&pt[thread_index], NULL, getBatch, (void*)(para+thread_index));
                break;
            case symmetric:
                pthread_create(&pt[thread_index], NULL, getSymmetricBatch, (void*)(para+thread_index));
                break;
        }
    }
    for (INT thread_index = 0; thread_index < workThreads; thread_index++)
        pthread_join(pt[thread_index], NULL);
    free(pt);
    free(para);
}

extern // "C"
void sampling(INT *batch_h, INT *batch_t, INT *batch_r, REAL *batch_y, INT batchSize, INT negRate, INT negRelRate, INT headBatchFlag) {
    sampling(batch_h, batch_t, batch_r, batch_y, batchSize, negRate, negRelRate, headBatchFlag, none, 0);
}

int main() {
	importTrainFiles();
	return 0;
}

