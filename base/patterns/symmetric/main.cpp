// #include "../../Setting.h"
// #include "../../Random.h"
// #include "../../Reader.h"
// #include "../../Corrupt.h"
// // #include "Test.h"
// // #include "Valid.h"
// // #include <cstdlib>
// // #include <pthread.h>
// #include "../../main.h"
// #include "../main.h"
// 
// #include "reader.h"
// 
// void* getSymmetricBatch(void* con) {
// 	Parameter *para = (Parameter *)(con);
// 	INT thread_index = para -> id;
// 	INT *batch_h = para -> batch_h;
// 	INT *batch_t = para -> batch_t;
// 	INT *batch_r = para -> batch_r;
// 	REAL *batch_y = para -> batch_y;
// 	INT batchSize = para -> batchSize;
// 	INT n_negative_triples_per_positive = para -> negRate;
// 	INT n_negative_triples_with_corrupted_relation_per_positive = para -> negRelRate;
// 	INT headBatchFlag = para -> headBatchFlag;
// 	INT first_triple_index, last_triple_index;
//     std::vector<PatternInstance>** patternInstanceSets = para -> patternInstanceSets;
//     // patternInstanceSets = *(para -> patternInstanceSets);
//     int nObservedTriplesPerPatternInstance = para -> nObservedTriplesPerPatternInstance;
// 
// 	if (batchSize % workThreads == 0) {
// 		first_triple_index = thread_index * (batchSize / workThreads);
// 		last_triple_index = (thread_index + 1) * (batchSize / workThreads);
// 	} else {
// 		first_triple_index = thread_index * (batchSize / workThreads + 1); // round up?
// 		last_triple_index = (thread_index + 1) * (batchSize / workThreads + 1);
// 		if (last_triple_index > batchSize) last_triple_index = batchSize;
// 	}
// 	REAL head_corruption_threshold = 500;
//     INT patternComponentOffset = batchSize * (1 + n_negative_triples_per_positive + n_negative_triples_with_corrupted_relation_per_positive);
// 
// 	for (INT current_triple_index = first_triple_index; current_triple_index < last_triple_index; current_triple_index++) {
//         // Sample a positive triple randomly
// 		// INT sampled_triple_index = rand_max(thread_index, symmetricTriples.size());
//         // cout << "Samling triple index (max value = " << symmetricTriplePatternInstances[nObservedTriplesPerPatternInstance].size() << ")..." << endl;
//         // cout << "Address of the symmetric pattern instance sets array: " << &symmetricTriplePatternInstances << endl;
//         // cout << "Address of given pattern instance sets array: " << para -> patternInstanceSets << endl;
//         // cout << "An example of a pattern instance: " << endl;
//         // patternInstanceSets[nObservedTriplesPerPatternInstance][0].print("kk");
//         // cout << "An example of a pattern instance: " << endl;
//         // (*patternInstanceSets[nObservedTriplesPerPatternInstance])[0].print("kks");
//         // cout << "Samling triple index (max value = " << (*(patternInstanceSets[nObservedTriplesPerPatternInstance])).size() << "..." << endl; // (*(para -> patternInstanceSets))[nObservedTriplesPerPatternInstance].size() << ")..." << endl;
// 		// INT sampled_triple_index = rand_max(thread_index, symmetricTriplePatternInstances[nObservedTriplesPerPatternInstance].size());
// 		// INT sampled_triple_index = rand_max(thread_index, (*(para -> patternInstanceSets))[nObservedTriplesPerPatternInstance].size());
// 		INT sampled_triple_index = rand_max(thread_index, (*(patternInstanceSets[nObservedTriplesPerPatternInstance])).size());
//         // cout << "Samled triple index" << endl;
//         // std::vector<Triple> sampledPatternOccurrence = symmetricTriples[sampled_triple_index];
//         // PatternInstance sampledPatternInstance = symmetricTriples[sampled_triple_index];
//         // cout << "Getting sampled pattern instance (with index " << sampled_triple_index << ")..." << endl;
//         // PatternInstance sampledPatternInstance = symmetricTriplePatternInstances[nObservedTriplesPerPatternInstance][sampled_triple_index];
//         // PatternInstance sampledPatternInstance = (*(para -> patternInstanceSets))[nObservedTriplesPerPatternInstance][sampled_triple_index];
//         PatternInstance sampledPatternInstance = (*(patternInstanceSets[nObservedTriplesPerPatternInstance]))[sampled_triple_index];
//         // cout << "Got sampled pattern instance" << endl;
//         INT sampledTripleIndex = 0;
// 
//         for (Triple sampledTriple: sampledPatternInstance.triples) {
//             batch_h[patternComponentOffset * sampledTripleIndex + current_triple_index] = sampledTriple.h;
//             batch_t[patternComponentOffset * sampledTripleIndex + current_triple_index] = sampledTriple.t;
//             batch_r[patternComponentOffset * sampledTripleIndex + current_triple_index] = sampledTriple.r;
//             batch_y[patternComponentOffset * sampledTripleIndex + current_triple_index] = 1;
//             INT last = batchSize;
//             // Sample negative triples
//             for (INT negative_triple_index = 0; negative_triple_index < n_negative_triples_per_positive; negative_triple_index++) {
//                 if (!crossSamplingFlag){
//                     if (bernFlag) // flag for considering a portion of triples with unique head/tail for those of which there is a given relationship
//                         head_corruption_threshold = 1000 * right_mean[sampledTriple.r] / (right_mean[sampledTriple.r] + left_mean[sampledTriple.r]);
//                     if (randd(thread_index) % 1000 < head_corruption_threshold) { // Corrupt TAIL by generating a random number
//                         batch_h[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = sampledTriple.h;
//                         batch_t[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = corrupt_head(thread_index, sampledTriple.h, sampledTriple.r);
//                         batch_r[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = sampledTriple.r;
//                     } else { // Corrupt HEAD
//                         batch_h[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = corrupt_tail(thread_index, sampledTriple.t, sampledTriple.r);
//                         batch_t[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = sampledTriple.t;
//                         batch_r[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = sampledTriple.r;
//                     }
//                     batch_y[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = -1;
//                     last += batchSize; // There will be <batchSize> triples generated by other threads which must be skipped
//                 } else  {
//                     if(headBatchFlag){ // Corrupt HEAD by using provided flag which specifies which part of triple must be corrupted
//                         batch_h[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = corrupt_tail(thread_index, sampledTriple.t, sampledTriple.r);
//                         batch_t[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = sampledTriple.t;
//                         batch_r[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = sampledTriple.r;
//                     } else { // Corrupt TAIL
//                         batch_h[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = sampledTriple.h;
//                         batch_t[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = corrupt_head(thread_index, sampledTriple.h, sampledTriple.r);
//                         batch_r[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = sampledTriple.r;
//                     }
//                     batch_y[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = -1;
//                     last += batchSize;
//                 }
//             } // end loop for each negative triple with corrupted entity
//             // Corrupt relations
//             for (INT negative_triple_index = 0; negative_triple_index < n_negative_triples_with_corrupted_relation_per_positive; negative_triple_index++) {
//                 batch_h[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = sampledTriple.h;
//                 batch_t[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = sampledTriple.t;
//                 batch_r[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = corrupt_rel(thread_index, sampledTriple.h, sampledTriple.t);
//                 batch_y[patternComponentOffset * sampledTripleIndex + current_triple_index + last] = -1;
//                 last += batchSize;
//             }
//             sampledTripleIndex += 1;
//         }
// 
//         // for (int observedTripleIndex = 0; observedTripleIndex < nObservedTriplesPerPatternInstance; observedTripleIndex++) {
//         INT observedTripleIndexCounter = 0;
//         for (INT observedTripleIndex: sampledPatternInstance.observedTripleIndices) {
//             if (observedTripleIndexCounter >= nObservedTriplesPerPatternInstance) {
//                 break;
//             }
// 
//             batch_h[patternComponentOffset * (sampledTripleIndex + observedTripleIndexCounter) + current_triple_index] = batch_h[observedTripleIndex * patternComponentOffset + current_triple_index];
//             batch_t[patternComponentOffset * (sampledTripleIndex + observedTripleIndexCounter) + current_triple_index] = batch_t[observedTripleIndex * patternComponentOffset + current_triple_index];
//             batch_r[patternComponentOffset * (sampledTripleIndex + observedTripleIndexCounter) + current_triple_index] = batch_r[observedTripleIndex * patternComponentOffset + current_triple_index];
//             batch_y[patternComponentOffset * (sampledTripleIndex + observedTripleIndexCounter) + current_triple_index] = batch_y[observedTripleIndex * patternComponentOffset + current_triple_index];
//             INT last = batchSize;
// 
//             for (INT negative_triple_index = 0; negative_triple_index < n_negative_triples_per_positive + n_negative_triples_with_corrupted_relation_per_positive; negative_triple_index++) {
//                 batch_h[patternComponentOffset * (sampledTripleIndex + observedTripleIndexCounter) + current_triple_index + last] = batch_h[patternComponentOffset * observedTripleIndex + current_triple_index + last];
//                 batch_t[patternComponentOffset * (sampledTripleIndex + observedTripleIndexCounter) + current_triple_index + last] = batch_t[patternComponentOffset * observedTripleIndex + current_triple_index + last];
//                 batch_r[patternComponentOffset * (sampledTripleIndex + observedTripleIndexCounter) + current_triple_index + last] = batch_r[patternComponentOffset * observedTripleIndex + current_triple_index + last];
//                 batch_y[patternComponentOffset * (sampledTripleIndex + observedTripleIndexCounter) + current_triple_index + last] = -1;
//                 last += batchSize;
//             }
// 
//             observedTripleIndexCounter++;
//         }
// 	} // end loop for each positive triple in the batch which should be processed by the thread
// 	pthread_exit(NULL);
// }

