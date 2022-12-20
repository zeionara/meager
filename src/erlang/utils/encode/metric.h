#ifndef ERLANG_UTILS_ENCODE_METRIC_H
#define ERLANG_UTILS_ENCODE_METRIC_H

#include "erl_nif.h"

#include "../../../base/evaluation/metric/tracker/Count.h"

namespace meager::erlang::utils::encode::metric {

    ERL_NIF_TERM metric(ErlNifEnv* env, main::evaluation::metric::tracker::TrackerBase* metric);

    ERL_NIF_TERM tree(ErlNifEnv* env, main::evaluation::metric::tree::Tree* tree, INT normalizationCoefficient);

}

#endif
