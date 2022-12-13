#ifndef EVALUATION_METRIC_TRACKER_TRACKER_H
#define EVALUATION_METRIC_TRACKER_TRACKER_H

#include <string>

#include "../score/Score.h"
#include "../Metric.h"

using namespace std;

namespace meager::main::evaluation::metric::tracker {

    struct TrackerBase {
        // string label;

        virtual void update(score::Score* score) = 0;
        virtual void reset() = 0;
        virtual string getLabel() = 0;
        virtual string getName() = 0;
        virtual double divide(INT divisor) = 0;
        virtual Metric getId() = 0;
    };

    template<typename T> struct Tracker: TrackerBase {

        T value;

        // MetricTracker(string label = "none") {
        Tracker() {
            value = 0;
            // this->label = label;
        }

        void reset() {
            value = 0;
        }

        double divide(INT divisor) {
            return double(value) / divisor;
        }

    };

}


#endif
