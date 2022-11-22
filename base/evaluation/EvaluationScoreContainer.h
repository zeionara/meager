#ifndef EVALUATION_EVALUATION_SCORE_CONTAINER
#define EVALUATION_EVALUATION_SCORE_CONTAINER

struct EvaluationScoreContainer {

    virtual void updateMetrics() = 0;
    virtual void resetScore() = 0;
    virtual void resetMetrics() = 0;
    virtual void reset() = 0;

};

#endif
