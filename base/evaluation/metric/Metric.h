#ifndef EVALUATION_METRIC_H
#define EVALUATION_METRIC_H

#include <string>
#include <stdexcept>

#define invalidArgument invalid_argument

using namespace std;

enum Metric { Count, Rank, ReciprocalRank };

Metric decodeMetric(string name);

#endif
