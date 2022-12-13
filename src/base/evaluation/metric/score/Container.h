#ifndef EVALUATION_METRIC_SCORE_CONTAINER
#define EVALUATION_METRIC_SCORE_CONTAINER

namespace meager::main::evaluation::metric::score {

    struct Container {

        virtual void updateMetrics() = 0;
        virtual void resetScore() = 0;
        virtual void resetMetrics() = 0;
        virtual void reset() = 0;

    };

}

#endif
