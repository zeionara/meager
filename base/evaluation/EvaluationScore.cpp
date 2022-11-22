#include "EvaluationScore.h"
#include "metric/MetricSetTracker.h"

void EvaluationScore::resetMetrics() {
    metrics->reset();
}

void EvaluationScore::updateMetrics() {
    metrics->update(this);
}
