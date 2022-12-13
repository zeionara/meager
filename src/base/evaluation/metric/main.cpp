#include <string>

#include "Metric.h"

#include "tracker/Count.h"
#include "tracker/Rank.h"
#include "tracker/ReciprocalRank.h"

namespace meager::main::evaluation::metric {

    Metric decodeName(string name) {

        if (name == count::name) {
            return Metric::Count;
        }

        if (name == rank::name) {
            return Metric::Rank;
        }

        if (name == reciprocal_rank::name) {
            return Metric::ReciprocalRank;
        }

        throw invalidArgument("Unknown metric name provided: " + name);
    }

}
