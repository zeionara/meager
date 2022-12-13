#ifndef EVALUATION_TRIPLE_CLASSIFICATION_EVALUATOR_H
#define EVALUATION_TRIPLE_CLASSIFICATION_EVALUATOR_H

#include <functional>

#include "../../corruption/Strategy.h"
#include "../../sampling/batch/Double.h"

#include "../../storage/Corpus.h"
#include "../../triple/list/ThickWrapper.h"
#include "../../triple/list/ThinWrapper.h"


namespace meager::main::evaluation::triple_classification{

    const string name = "triple_classification";

    template <typename T, typename R>
    struct ClassificationEvaluator {

        Corpus* corpus;

        triple::list::ThickWrapper<T>* triples;
        triple::list::ThinWrapper<T>* testTriples;

        corruption::Strategy<R>* corruptionStrategy;

        ClassificationEvaluator(Corpus* corpus, triple::list::ThickWrapper<T>* triples, triple::list::ThinWrapper<T>* testTriples) {
            this->corpus = corpus;
            this->triples = triples;
            this->testTriples = testTriples;
        }

        sampling::batch::Double* makeBatch() { // Generate all possible triples for every entity which may be used as a triple head
            triple::Triple* corruptedTriples = new triple::Triple[testTriples->content->length];
            // triple::Triple* corruptedTriples = (triple::Triple*) malloc(testTriples->content->length * sizeof(Triple));

            for (INT i = 0; i < testTriples->content->length; i++) {
                corruptedTriples[i] = corruptionStrategy->corrupt(testTriples->content->items[i]);
            }

            return new sampling::batch::Double(
                new sampling::batch::Triple(testTriples->content, testTriples->content->length),  // positive
                new sampling::batch::Triple(corruptedTriples, testTriples->content->length)  // negative
            );
        }

        void getBestThreshold(REAL *relThresh, REAL *positiveScore, REAL *negativeScore, REAL thresholdStep = 0.01) {
            REAL minScore, maxScore, bestThreshold, hypothesisThreshold, bestAccuracy, accuracy;
            INT nTicks, nCorrect, nTriples;

            for (INT relation = 0; relation < corpus->countRelations(); relation++) {
                if (testTriples->content->left[relation] == -1) continue;  // If there are no triples with such relation then skip it

                nTriples = (testTriples->content->right[relation] - testTriples->content->left[relation] + 1) * 2;
                
                // 1. Get min and max score assigned to positive or negative triple from the test subset

                minScore = positiveScore[testTriples->content->left[relation]];  // Score for the first triple having this relation
                if (negativeScore[testTriples->content->left[relation]] < minScore) {
                    minScore = negativeScore[testTriples->content->left[relation]];  // If score for corrupted triple is less then take this score
                }

                maxScore = positiveScore[testTriples->content->left[relation]];
                if (negativeScore[testTriples->content->left[relation]] > maxScore) {
                    maxScore = negativeScore[testTriples->content->left[relation]];
                }

                for (INT i = testTriples->content->left[relation] + 1; i <= testTriples->content->right[relation]; i++) {
                    if(positiveScore[i] < minScore) minScore = positiveScore[i];
                    if(negativeScore[i] < minScore) minScore = negativeScore[i];

                    if(positiveScore[i] > maxScore) maxScore = positiveScore[i];
                    if(negativeScore[i] > maxScore) maxScore = negativeScore[i];
                }

                // 2. Choose threshold which maximizes classifier accuracy

                nTicks = INT((maxScore - minScore)/thresholdStep);

                for (INT i = 0; i <= nTicks; i++) {
                // for (INT hypothesisThreshold = minScore; hypothesisThreshold <= maxScore; hypothesisThreshold += thresholdStep) {
                    hypothesisThreshold = minScore + i * thresholdStep;
                    nCorrect = 0;
                    for (INT j = testTriples->content->left[relation]; j <= testTriples->content->right[relation]; j++) {
                        if (positiveScore[j] <= hypothesisThreshold) nCorrect ++; // Classifier generates score = (1 - probability)
                        if (negativeScore[j] > hypothesisThreshold) nCorrect ++;
                        // if (positiveScore[j] > hypothesisThreshold) nCorrect++;
                        // if (negativeScore[j] <= hypothesisThreshold) nCorrect++;
                    }

                    accuracy = 1.0 * nCorrect / nTriples;

                    if (i == 0) {
                        bestThreshold = hypothesisThreshold;
                        bestAccuracy = accuracy;
                    } else if (accuracy > bestAccuracy) {
                        bestAccuracy = accuracy;
                        bestThreshold = hypothesisThreshold;
                    }
                }

                relThresh[relation] = bestThreshold;
            }
        }

        REAL evaluate(REAL *relationThresholds, REAL *positiveScores, REAL *negativeScores) {
            REAL* accuracy = (REAL *)calloc(corpus->countRelations(), sizeof(REAL));
            INT nCorrectTotal = 0, nTriplesTotal = 0, nTriples;
            INT nCorrect = 0; // , nTriples = 0;
            REAL meanAccuracy;

            for (INT relation = 0; relation < corpus->countRelations(); relation++) {
                if (testTriples->content->left[relation] == -1 ) continue; // || testList->content->left[r] ==-1

                nCorrect = 0;

                nTriples = (testTriples->content->right[relation] - testTriples->content->left[relation] + 1) * 2;

                for (INT i = testTriples->content->left[relation]; i <= testTriples->content->right[relation]; i++) {
                    if (positiveScores[i] <= relationThresholds[relation]) nCorrect++;  // Classifier generates score = (1 - probability)
                    if (negativeScores[i] > relationThresholds[relation]) nCorrect++;
                    // total += 2;
                }

                accuracy[relation] = 1.0 * nCorrect / nTriples;

                nCorrectTotal += nCorrect; 
                nTriplesTotal += nTriples;
            }
            meanAccuracy = 1.0 * nCorrectTotal / nTriplesTotal;
            printf("triple classification accuracy is %lf\n", meanAccuracy);

            return meanAccuracy;
        }
    };

}

#endif
