#ifndef ERLANG_READER_H

#define ERLANG_READER_H
#include "erl_nif.h"

extern ERL_NIF_TERM
import_train_files(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

#endif

