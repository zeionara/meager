#ifndef EVALUATION_EVALUATION_SCORE_GROUP
#define EVALUATION_EVALUATION_SCORE_GROUP

#include "EvaluationScore.h"
#include "EvaluationScoreContainer.h"

#include "metric/MetricSetTracker.h"

struct EvaluationScoreGroup: EvaluationScoreContainer {

    EvaluationScore* filtered;
    EvaluationScore* unfiltered;

    EvaluationScoreGroup() {
        // MetricSetTracker* tracker = new MetricSetTracker();
        // cout << "Created metric set tracker " << tracker << endl;
        filtered = new EvaluationScore(new MetricSetTracker());
        // cout << "Written metric set tracker " << filtered->metrics << endl;
        unfiltered = new EvaluationScore(new MetricSetTracker());
    }

    void updateMetrics() {
        // cout << "Updating metrics in group" << endl;
        filtered->updateMetrics();
        // cout << filtered->metrics << endl;
        // cout << filtered->metrics->trackers << endl;
        // filtered->metrics->update(filtered);
        // cout << "Updating metrics in group" << endl;
        unfiltered->updateMetrics();
    }

    void resetScore() {
        filtered->resetValue();
        unfiltered->resetValue();
    }

    void resetMetrics() {
        filtered->resetMetrics();
        unfiltered->resetMetrics();
    }

    void reset() {
        filtered->reset();
        unfiltered->reset();
    }

    void printMetrics(string prefix, INT nTriples) {
        filtered->metrics->printMetrics(prefix + " filtered", nTriples);
        filtered->metrics->printMetrics(prefix + " unfiltered", nTriples);
    }

};

#endif
