#include "Tracker.h"

namespace meager::main::evaluation::metric::tracker {

    double operator/(TrackerBase& tracker, INT& divisor) {
        return tracker.valueAsDouble() / divisor;
    }

}
