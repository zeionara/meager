#ifndef ERLANG_UTILS_DECODE_METRIC_H
#define ERLANG_UTILS_DECODE_METRIC_H

#include "../../../base/evaluation/metric/tracker/Tracker.h"

#include "../main.h"

namespace meager::erlang::utils::decode::metric {

    List<main::evaluation::metric::tracker::TrackerBase*>* metrics(ErlNifEnv *env, ERL_NIF_TERM metrics);
}

#endif
