#include "erl_nif.h"
#include "utils.h"

#include "../base/utils/main.h"
#include "../base/api/evaluator.h"

enum Metric { Count, Rank, ReciprocalRank };

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

    throw invalidArgument("Unknown metric name provided");
}


struct MetricConfig {
    Metric metric;
    INT topN;
    
    MetricConfig(Metric metric, INT topN = -1) {
        switch (metric) {
            case Count:
                if (topN < 0)
                    throw invalidArgument("Metric count must have top-n parameter");
                break;
            case Rank:
            case ReciprocalRank:
                if (topN > -1)
                    throw invalidArgument("Only metric count is allowed to have top-n parameter");
        }

        this->metric = metric;
        this->topN = topN;
    }

};

extern ERL_NIF_TERM
initEvaluator_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    try {
        // unsigned nMetrics;

        // enif_get_list_length(env, argv[0], &nMetrics);
       
        // cout << "N metrics = " << nMetrics << endl;

        // MetricConfig* metrics = (MetricConfig*)malloc(nMetrics * sizeof(MetricConfig));

        List<MetricConfig>* metrics = decodeList<MetricConfig>(env, argv[0], [](ErlNifEnv* env, ERL_NIF_TERM metric) {
        // decodeList<MetricConfig>(env, argv[0], metrics, nMetrics, [](ErlNifEnv* env, ERL_NIF_TERM metric) {
            // cout << "Started decoding metric" << endl;

            // INT const descriptionLength = 2;

            // cout << "Set arity" << endl;

            int arity; // = descriptionLength;

            // cout << "Preparing array" << endl;

            // const ERL_NIF_TERM** metricDescription = (const ERL_NIF_TERM**)malloc(descriptionLength * sizeof(ERL_NIF_TERM*));
            const ERL_NIF_TERM* metricDescription; // = (const ERL_NIF_TERM*)malloc(descriptionLength * sizeof(ERL_NIF_TERM));

            // cout << "Getting tuple..." << endl;

            enif_get_tuple(env, metric, &arity, &metricDescription);

            // cout << "Finished decoding metric" << endl;

            // char* metricName = enif_get_atom_(env, *metricDescription[0]);
            char* metricName = enif_get_atom_(env, metricDescription[0]);

            // cout << "Metric name = " << metricName << endl;

            // INT topN = enif_get_long_(env, *metricDescription[1]);
            INT topN = enif_get_long_(env, metricDescription[1]);

            // cout << "Top n = " << topN << endl;

            return MetricConfig(decodeMetric(metricName), topN);
        });

        SubsetType subset = decodeSubsetType(enif_get_atom_(env, argv[2]));
        bool verbose = enif_get_bool(env, argv[3]);

        // initEvaluator([nMetrics, metrics](){
        initEvaluator([metrics](){
            // INT const length = nMetrics;
            // INT i = 0;

            // MetricTrackerBase** trackers = (MetricTrackerBase**) malloc(length * sizeof(MetricTrackerBase*));
            MetricTrackerBase** trackers = (MetricTrackerBase**) malloc(metrics->length * sizeof(MetricTrackerBase*));

            for (INT i = 0; i < metrics->length; i++) {
                MetricConfig metric = metrics->items[i];

                switch (metric.metric) {
                    case Count:
                        trackers[i] = new CountMetricTracker(metric.topN); 
                        break;
                    case Rank:
                        trackers[i] = new RankMetricTracker(); 
                        break;
                    case ReciprocalRank:
                        trackers[i] = new ReciprocalRankMetricTracker(); 
                        break;
                }
            }

            return new MetricSetTracker(trackers, metrics->length);
        }, subset, verbose);

    } catch (invalidArgument& e) {
        return completed_with_error(env, e.what());
    }

    return completed_with_success(env);
}
