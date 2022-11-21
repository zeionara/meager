#ifndef EVALUATION_EVALUATOR_H
#define EVALUATION_EVALUATOR_H

#include <functional>

#include "../Setting.h"
#include "../triple/list/ThickTripleListWrapper.h"
#include "../triple/list/ThinTripleListWrapper.h"
#include "../samplers/TripleBatch.h"
#include "../corruption/CorruptionStrategy.h"


struct MetricSetTracker;

struct EvaluationScore {
    INT value;
    MetricSetTracker* metrics;

    EvaluationScore(MetricSetTracker* metrics) {
        value = 0;
        this->metrics = metrics;
    }

    // finalize() {
    //     if (l_s < 10) l_tot += 1;
    //     if (l_s < 3) l3_tot += 1;
    //     if (l_s < 1) l1_tot += 1;

    //     if (l_filter_s < 10) l_filter_tot += 1;
    //     if (l_filter_s < 3) l3_filter_tot += 1;
    //     if (l_filter_s < 1) l1_filter_tot += 1;

    //     if (l_filter_s_constrain < 10) l_filter_tot_constrain += 1;
    //     if (l_filter_s_constrain < 3) l3_filter_tot_constrain += 1;
    //     if (l_filter_s_constrain < 1) l1_filter_tot_constrain += 1;

    //     if (l_s_constrain < 10) l_tot_constrain += 1;
    //     if (l_s_constrain < 3) l3_tot_constrain += 1;
    //     if (l_s_constrain < 1) l1_tot_constrain += 1;
    // }
    //
    void resetValue() {
        value = 0;
    }

    void resetMetrics();
    void updateMetrics();

    void reset() {
        resetValue();
        resetMetrics();
    };
};

struct MetricTrackerBase {
    virtual void update(EvaluationScore* score) = 0;
    virtual void reset() = 0;
};

template<typename T> struct MetricTracker: MetricTrackerBase {
    T value;

    MetricTracker() {
        value = 0;
    }

    void reset() {
        value = 0;
    }

    // virtual void update(EvaluationScore* score) = 0;
};


struct CountMetricTracker: MetricTracker<INT> {
    INT n;

    CountMetricTracker(INT n): MetricTracker() {
        this->n = n;
    }

    void update(EvaluationScore* score) {
        if (score->value < n) {
            value += 1;
        }
    }
};

struct RankMetricTracker: MetricTracker<INT> {
    RankMetricTracker(): MetricTracker() {};

    void update(EvaluationScore* score) {
        value += score->value + 1;
    }
};

struct ReciprocalRankMetricTracker: MetricTracker<REAL> {
    ReciprocalRankMetricTracker(): MetricTracker() {};

    void update(EvaluationScore* score) {
        value += 1.0 / (score->value + 1);
    }
};

struct MetricSetTracker {
    MetricTrackerBase* trackers;
    INT length;

    MetricSetTracker() {
        MetricTrackerBase* trackers[5];

        trackers[0] = new CountMetricTracker(1);
        trackers[1] = new CountMetricTracker(3);
        trackers[2] = new CountMetricTracker(10);
        trackers[3] = new RankMetricTracker();
        trackers[4] = new ReciprocalRankMetricTracker();

        length = 5;
    }

    void update(EvaluationScore* score) {
        for (INT i = 0; i < length; i++) {
            this->trackers[i].update(score);
        }
    }

    void reset() {
        for (INT i = 0; i < length; i++) {
            this->trackers[i].reset();
        }
    }
};


struct EvaluationScoreGroup {
    EvaluationScore* filtered;
    EvaluationScore* unfiltered;

    EvaluationScoreGroup() {
        filtered = new EvaluationScore(new MetricSetTracker());
        unfiltered = new EvaluationScore(new MetricSetTracker());
    }

    void reset() {
        filtered->reset();
        unfiltered->reset();
    }

    void updateMetrics() {
        filtered->updateMetrics();
        unfiltered->updateMetrics();
    }
};

struct EvaluationState {
    INT lastHead;
    INT lastTail;

    EvaluationScoreGroup* constrained;
    EvaluationScoreGroup* unconstrained;

    EvaluationState() {
        lastHead = 0;
        lastTail = 0;

        constrained = new EvaluationScoreGroup();
        unconstrained = new EvaluationScoreGroup();
    }

    void resetScore() {
        constrained->reset();
        unconstrained->reset();
    }

    void updateMetrics() {
        constrained->updateMetrics();
        unconstrained->updateMetrics();
    }
};

void EvaluationScore::resetMetrics() {
    metrics->reset();
}

void EvaluationScore::updateMetrics() {
    metrics->update(this);
}

struct Evaluator {
    EvaluationState* state;

    ThickTripleListWrapper* triples;
    ThinTripleListWrapper* testTriples;

    CorruptionStrategy* corruptionStrategy;

    TripleBatch getHeadBatch() {
        return getBatch(
            [&](INT i){return Triple(i, testTriples->content->items[state->lastHead].r, testTriples->content->items[state->lastHead].t);}
        );
    }

    TripleBatch getTailBatch() {
        return getBatch(
            [&](INT i){return Triple(testTriples->content->items[state->lastHead].h, testTriples->content->items[state->lastHead].r, i);}
        );
    }


    void evaluate(
        REAL *probabilities, bool reverse, INT lastTripleComponent, INT (*getTripleComponent)(Triple triple), Triple (*makeTriple)(INT hypothesis, Triple triple),
        function<bool(Triple)> isTripleFromDataset, function<bool(Triple)> isCorrectTriple
    ) {
        Triple reference = testTriples->content->items[lastTripleComponent];

        REAL reference_distance = probabilities[getTripleComponent(reference)];

        state->resetScore();

        for (INT hypothesis = 0; hypothesis < triples->frequencies->nEntities; hypothesis++) {
            if (hypothesis != getTripleComponent(reference)) { 
                Triple sampledTriple = makeTriple(hypothesis, reference);
                REAL hypothesis_distance = probabilities[hypothesis];
                if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                    state->unconstrained->unfiltered += 1; // Count incorrectly classified triples
                    if (not isTripleFromDataset(sampledTriple)) // If less probable triple is not present in the dataset
                        state->unconstrained->filtered += 1; // Count incorrectly classified triples which are not present in the dataset (filtered score must be better than unfiltered concerning rank)
                }

                if (isCorrectTriple(sampledTriple)) {
                    if ((!reverse && (hypothesis_distance <= reference_distance)) || (reverse && (hypothesis_distance >= reference_distance))) {
                        state->constrained->unfiltered += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads (constrained score must be better than unconstrained)
                        if (not isTripleFromDataset(sampledTriple)) {
                            state->constrained->filtered += 1; // Count incorrectly classified triples the head of which is presented in type constraint list for heads but triple does not exist
                        }
                    }
                }
            }
        }

        state->updateMetrics();
        state->lastHead++;
    }

    private: TripleBatch getBatch(function<Triple(INT)> makeTriple) { // Generate all possible triples for every entity which may be used as a triple head
        Triple* triples = (Triple*) malloc(this->triples->frequencies->nEntities * sizeof(Triple));

        for (INT i = 0; i < this->triples->frequencies->nEntities; i++) {
            triples[i] = makeTriple(i);
        }

        return TripleBatch(triples, this->triples->frequencies->nEntities);
    }

};

#endif
