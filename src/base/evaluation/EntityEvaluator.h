#ifndef EVALUATION_ENTITY_EVALUATOR_H
#define EVALUATION_ENTITY_EVALUATOR_H

#include <chrono>

#include "../triple/list/ThickWrapper.h"
#include "../triple/list/ThinWrapper.h"
#include "../samplers/TripleBatch.h"
// #include "../corruption/Strategy.h"
#include "EvaluationScoreCluster.h"
#include "../storage/Corpus.h"

template <typename T>
struct EntityEvaluator {

    INT currentTripleIndex;

    EvaluationScoreCluster* state;
    Corpus* corpus;

    triple::list::ThickWrapper<T>* triples;
    triple::list::ThinWrapper<T>* testTriples;

    string label;

    // virtual bool isTripleFromDataset(Triple triple) = 0;
    // virtual bool isCorrectTriple(Triple triple) = 0;
   
    EntityEvaluator(Corpus* corpus, triple::list::ThickWrapper<T>* triples, triple::list::ThinWrapper<T>* testTriples, MetricSetTrackerMaker makeMetricSetTracker, string label) {
        this->corpus = corpus;
        this->triples = triples;
        this->testTriples = testTriples;
        this->state = new EvaluationScoreCluster(makeMetricSetTracker, label);
        this->currentTripleIndex = 0;
        this->label = label;
    }

    TripleBatch* makeBatch() { // Generate all possible triples for every entity which may be used as a triple head
        // cout << "make batch" << endl;
        // cout << this->testTriples->content->items[this->currentTripleIndex].h << " " << this->testTriples->content->items[this->currentTripleIndex].r << " " << this->testTriples->content->items[this->currentTripleIndex].t << endl;
        // cout << "--" << endl;
        triple::Triple* triples = (triple::Triple*) malloc(this->corpus->countEntities() * sizeof(triple::Triple));
        // cout << "made batch" << endl;

        INT nEntities = this->corpus->countEntities();
        // cout << "counted entities" << endl;

        for (INT i = 0; i < nEntities; i++) {
            // cout << i << endl;
            triples[i] = makeTriple(i);
            // cout << triples[i].h << " " << triples[i].r << " " << triples[i].t << endl;
        }
        // cout << "made triples" << endl;

        return new TripleBatch(triples, this->corpus->countEntities());
    }

    void evaluate(REAL *probabilities, bool reverse, bool verbose = false) {
        // cout << "Test triples @ " << currentTripleIndex << endl;
        // cout << testTriples << endl;

        triple::Triple reference = testTriples->content->items[currentTripleIndex];

        // cout << "Reference @ " << getTripleComponent(reference) <<  endl;

        REAL reference_distance = probabilities[getTripleComponent(reference)];

        // cout << state->unconstrained->unfiltered->value;
        state->resetScore();
        //

        // auto tripleMakingStopWatch = new Stopwatch(this->corpus->countEntities());
        // auto unconstrainedStopwatch = new Stopwatch(this->corpus->countEntities());
        // auto constrainedStopwatch = new Stopwatch(this->corpus->countEntities());
        // auto tripleComponentStopwatch = new Stopwatch(this->corpus->countEntities());
        // auto loopStopwatch = new Stopwatch(1);

        // cout << "Start evaluation loop" << endl;
        // for (INT hypothesis = 0; hypothesis < this->corpus->countEntities(); hypothesis++) {
        // loopStopwatch->start();
        for (INT hypothesis = 0; hypothesis < this->corpus->countEntities(); hypothesis++) {

            // tripleComponentStopwatch->start();
            auto tripleComponent = getTripleComponent(reference);
            // tripleComponentStopwatch->stop();

            if (hypothesis != tripleComponent) { 

                // tripleMakingStopWatch->start();

                triple::Triple sampledTriple = makeTriple(hypothesis);

                // tripleMakingStopWatch->stop();

                // unconstrainedStopwatch->start();

                REAL hypothesis_distance = probabilities[hypothesis];
                if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                    state->unconstrained->unfiltered->value += 1; // Count incorrectly classified triples
                    if (not corpus->contains(sampledTriple)) // If less probable triple is not present in the dataset
                        state->unconstrained->filtered->value += 1; // Count incorrectly classified triples which are not present in the dataset (filtered score must be better than unfiltered concerning rank)
                }

                // unconstrainedStopwatch->stop();

                // constrainedStopwatch->start();
                auto allows = corpus->allows(sampledTriple);
                // constrainedStopwatch->stop();

                if (allows) {
                    if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                        state->constrained->unfiltered->value += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads (constrained score must be better than unconstrained)
                        if (not corpus->contains(sampledTriple)) {
                            state->constrained->filtered->value += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads but triple does not exist
                        }
                    }
                }

            }
        }
        // loopStopwatch->stop();

        // cout << endl << endl;
        // loopStopwatch->print("evaluation loop completion");
        // tripleComponentStopwatch->print("triple component search");
        // tripleMakingStopWatch->print("triple making");
        // unconstrainedStopwatch->print("unconstrained triple check");
        // constrainedStopwatch->print("constrained triple check");
        // cout << endl << endl;

        // cout << "Making triple in " << tripleMakingStopWatch->mean() << " sum = " << tripleMakingStopWatch->sum() << " ms" << endl;
        // cout << "Handle unconstrained part in " << unconstrainedStopwatch->mean() << " ms" << endl;
        // cout << "Handle constrained part in " << constrainedStopwatch->mean() << " ms" << endl;
        // cout << "Found triple component " << tripleComponentStopwatch->mean() << " ms" << endl;

        // cout << "Updating metrics" << endl;

        // cout << state << endl;

        state->updateMetrics();

        // cout << "Updated metrics" << endl;

        currentTripleIndex++;
    }

    private:

    virtual triple::Triple makeTriple(INT i) = 0;
    virtual INT getTripleComponent(triple::Triple triple) = 0;

};

#endif
