#include "erl_nif.h"
#include "setting.h"
#include "random.h"
#include "reader.h"

static ErlNifFunc meager_nif_funcs[] = {
    {"import_train_files", 2, import_train_files},
    {"set_in_path", 4, set_in_path},
    {"set_bern", 1, set_bern},
    {"set_work_threads", 1, set_work_threads},
    {"rand_reset", 0, rand_reset}
};

ERL_NIF_INIT(Elixir.Meager, meager_nif_funcs, NULL, NULL, NULL, NULL)

