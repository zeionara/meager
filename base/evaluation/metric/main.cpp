#include <string>

#include "Metric.h"

Metric decodeMetric(string name) {

    if (name == "count") {
        return Count;
    }

    if (name == "rank") {
        return Rank;
    }

    if (name == "reciprocal_rank") {
        return ReciprocalRank;
    }

    throw invalidArgument("Unknown metric name provided: " + name);
}
