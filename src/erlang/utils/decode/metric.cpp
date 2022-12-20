#include "../../../base/evaluation/metric/main.h"

#include "../../../base/evaluation/metric/tracker/Count.h"
#include "../../../base/evaluation/metric/tracker/Rank.h"
#include "../../../base/evaluation/metric/tracker/ReciprocalRank.h"

#include "../main.h"

#include "../nif/decode.h"

namespace meager::erlang::utils::decode::metric {

    List<main::evaluation::metric::tracker::TrackerBase*>* metrics(ErlNifEnv *env, ERL_NIF_TERM metrics) {
        return utils::nif::decode::list<main::evaluation::metric::tracker::TrackerBase*>(
            env, metrics, [](
                ErlNifEnv* env, ERL_NIF_TERM metric
            ) -> main::evaluation::metric::tracker::TrackerBase* {
                int length;
                const ERL_NIF_TERM* metricDescription;

                enif_get_tuple(env, metric, &length, &metricDescription);

                if (length < 1) {
                    throw invalidArgument("Empty metric descriptions are not allowed");
                }

                char* metricName = utils::nif::decode::atom(env, metricDescription[0]);

                switch (main::evaluation::metric::decodeName(metricName)) {
                    case main::evaluation::metric::Metric::Count:
                        if (length > 1)
                            return new main::evaluation::metric::tracker::Count(utils::nif::decode::longInteger(env, metricDescription[1])); 
                        throw invalidArgument("Metric count requires one parameter (top-n)");
                    case main::evaluation::metric::Metric::Rank:
                        if (length < 2)
                            return new main::evaluation::metric::tracker::Rank(); 
                        throw invalidArgument("Metric rank doesn't accept parameters");
                    case main::evaluation::metric::Metric::ReciprocalRank:
                        if (length == 1)
                            return new main::evaluation::metric::tracker::ReciprocalRank(); 
                        throw invalidArgument("Metric reciprocal-rank doesn't accept parameters");
                    default:
                        throw invalidArgument("Unspecified metric tracker for metric " + string(metricName));
                }
            }
        ); // return List(
    }
}
