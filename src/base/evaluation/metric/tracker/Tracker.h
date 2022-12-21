#ifndef EVALUATION_METRIC_TRACKER_TRACKER_H
#define EVALUATION_METRIC_TRACKER_TRACKER_H

#include <string>

#include "../score/Score.h"
#include "../Metric.h"

using namespace std;

namespace meager::main::evaluation::metric::tracker {

    struct TrackerBase {

        virtual void update(score::Score* score) = 0;
        virtual void reset() = 0;
        virtual string getLabel() = 0;
        virtual string getName() = 0;
        // virtual double divide(INT divisor) = 0;
        virtual double valueAsDouble() = 0;
        virtual Metric getId() = 0;

    };

    template<typename T>
    struct Tracker: TrackerBase {

        T value;

        Tracker() {
            value = 0;
        }

        void reset() {
            value = 0;
        }

        double valueAsDouble() {
            return double(value);
        }

    };

    double operator/(TrackerBase& tracker, INT& divisor);

}


#endif
