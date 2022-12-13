#ifndef EVALUATION_METRIC_METRIC_H
#define EVALUATION_METRIC_METRIC_H

#include <string>
#include <stdexcept>

#define invalidArgument invalid_argument

using namespace std;

namespace meager::main::evaluation::metric {

    enum class Metric {
        Count,
        Rank,
        ReciprocalRank
    };

}

#endif
