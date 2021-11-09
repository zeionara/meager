#include "erl_nif.h"
#include "setting.h"

static ErlNifFunc meager_nif_funcs[] = {
    {"set_in_path", 4, set_in_path}
};

ERL_NIF_INIT(Elixir.Meager, meager_nif_funcs, NULL, NULL, NULL, NULL)

