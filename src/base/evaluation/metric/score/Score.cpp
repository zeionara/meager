// #include "../tracker/Tracker.h"
#include "../tracker/Set.h"

namespace meager::main::evaluation::metric::score {

    void Score::resetMetrics() {
        metrics->reset();
    }

    void Score::updateMetrics() {
        metrics->update(this);
    }

}
