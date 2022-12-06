#include <string>

#include "Metric.h"

#include "CountMetricTracker.h"
#include "RankMetricTracker.h"
#include "ReciprocalRankMetricTracker.h"

Metric decodeMetric(string name) {

    if (name == COUNT) {
        return Count;
    }

    if (name == RANK) {
        return Rank;
    }

    if (name == RECIPROCAL_RANK) {
        return ReciprocalRank;
    }

    throw invalidArgument("Unknown metric name provided: " + name);
}
