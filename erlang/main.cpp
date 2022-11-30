#include "erl_nif.h"
#include "setting.h"
#include "random.h"
#include "reader.h"
#include "../base/types.h"
#include "utils.h"
// #include "../base/main.h"
#include <iostream>
#include "test.h"

#include <string>
#include <iostream>
#include <sstream>

#include "../base/samplers/PatternSampler.h"
#include "../base/api/corpus.h"

#include "corpus.h"

using namespace std;

extern ERL_NIF_TERM
sample(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // cout << "Start sampling" << endl;
    INT batch_size = enif_get_long_(env, argv[0]); 

    // cout << "batch size = " << batch_size << endl;

    INT entity_negative_rate = enif_get_long_(env, argv[1]); 
    INT relation_negative_rate = enif_get_long_(env, argv[2]); 
    int n_observed_triples_per_pattern_instance = enif_get_int_(env, argv[5]); 
    char* pattern_name = enif_get_atom_(env, argv[6]);

    INT head_batch_flag = 0;
    
    if (enif_get_bool(env, argv[3], argv[4])) {
        head_batch_flag = 1;
    }

    // cout << "HEAD BATCH FLAG = " << head_batch_flag << endl;

    Pattern pattern = decodePatternName(pattern_name);
    PatternDescription patternDescription = corpus->train->patterns->get(pattern); // patternDescriptions[pattern];

    int nTriplesPerPatternInstance = patternDescription.nTriplesPerInstance;

    // cout << nTriplesPerPatternInstance << endl;

    // unordered_map<string, PatternDescription>::const_iterator patternDescriptionIterator = patternDescriptions.find(pattern_name);

    // if (patternDescriptionIterator != patternDescriptions.end()) {
    //     nTriplesPerPatternInstance = patternDescriptionIterator->second.nTriplesPerInstance;
    // }

    if (n_observed_triples_per_pattern_instance > nTriplesPerPatternInstance) {
        // stringstream message;
        cout << "Requested number of observed triples exceeds number of triples per instance of the selected pattern (" << n_observed_triples_per_pattern_instance <<
            " > " << nTriplesPerPatternInstance << "); using the highest available value (" << nTriplesPerPatternInstance << ")" << endl;
        // cout << message.str();
        // return completed_with_error(env, &message);
        n_observed_triples_per_pattern_instance = nTriplesPerPatternInstance;
    }

    if (n_observed_triples_per_pattern_instance < 0) {
        cout << "Requested number of observed triples is negative; using the lowest available value (0)" << endl;
        n_observed_triples_per_pattern_instance = 0;
    }

    int foo = (nTriplesPerPatternInstance + n_observed_triples_per_pattern_instance) * batch_size;
    int batch_tuple_size = (nTriplesPerPatternInstance + n_observed_triples_per_pattern_instance) * batch_size * (1 + entity_negative_rate + relation_negative_rate);

    // cout << "batch tuple size = " << foo << endl;

    ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_y = new ERL_NIF_TERM[batch_tuple_size]();

    INT* batch_h_encoded = new INT[batch_tuple_size]();
    INT* batch_t_encoded = new INT[batch_tuple_size]();
    INT* batch_r_encoded = new INT[batch_tuple_size]();
    REAL* batch_y_encoded = new REAL[batch_tuple_size]();
    
    // if (patternDescriptionIterator == patternDescriptions.end()) {
    //     stringstream message;
    //     message << "Selected an unknown pattern '" << pattern_name << "'. Available patterns: ";
    //     int counter = 0;
    //     for (auto patternDescriptionEntry: patternDescriptions) {
    //         if (counter++ > 0) {
    //             message << ", "; 
    //         }
    //         message << patternDescriptionEntry.first;
    //     }
    //     // message << endl;
    //     return completed_with_error(env, &message);
    // }

    // if (patternDescriptions[pattern_name].instanceSets[n_observed_triples_per_pattern_instance]->size() < 1) {
    if (patternDescription.instanceSets[n_observed_triples_per_pattern_instance]->size() < 1) {
        stringstream message;
        message << "Cannot sample " << pattern_name << " pattern instances in which there are at least " << n_observed_triples_per_pattern_instance << " observed triples since there are no observed sets of triples that satisfy these conditions";
        return completed_with_error(env, &message);
    }

    PatternSampler<long>* sampler = new PatternSampler<INT>(pattern, n_observed_triples_per_pattern_instance, false, false, 8);

    // cout << "Created sampler" << endl;

    TripleBatch* tripleBatch = sampler->sample(corpus, batch_size, entity_negative_rate, relation_negative_rate, head_batch_flag);

    // cout << "Used sampler" << endl;

    // sampling(batch_h_encoded, batch_t_encoded, batch_r_encoded, batch_y_encoded, batch_size, entity_negative_rate, relation_negative_rate, head_batch_flag, "none", n_observed_triples_per_pattern_instance);

    // enif_encode_array_of_long(env, batch_h_encoded, batch_h, batch_tuple_size);
    // enif_encode_array_of_long(env, batch_t_encoded, batch_t, batch_tuple_size);
    // enif_encode_array_of_long(env, batch_r_encoded, batch_r, batch_tuple_size);
    // enif_encode_array_of_float(env, batch_y_encoded, batch_y, batch_tuple_size);

    enif_encode_array_of_long(env, tripleBatch->head, batch_h, batch_tuple_size);
    enif_encode_array_of_long(env, tripleBatch->tail, batch_t, batch_tuple_size);
    enif_encode_array_of_long(env, tripleBatch->relation, batch_r, batch_tuple_size);
    enif_encode_array_of_float(env, tripleBatch->labels, batch_y, batch_tuple_size);

    delete [] batch_h_encoded;
    delete [] batch_t_encoded;
    delete [] batch_r_encoded;
    delete [] batch_y_encoded;

    ERL_NIF_TERM* batch = new ERL_NIF_TERM[4]();

    batch[0] = enif_make_list_from_array(env, batch_h, batch_tuple_size);
    batch[1] = enif_make_list_from_array(env, batch_t, batch_tuple_size);
    batch[2] = enif_make_list_from_array(env, batch_r, batch_tuple_size);
    batch[3] = enif_make_list_from_array(env, batch_y, batch_tuple_size);

    delete [] batch_h;
    delete [] batch_t;
    delete [] batch_r;
    delete [] batch_y;
    
    ERL_NIF_TERM result = enif_make_list_from_array(env, batch, 4);

    delete [] batch;

    // cout << "Stop sampling" << endl;

    return completed_with_success(
        env,
        result
    );
}

static ErlNifFunc meager_nif_funcs[] = {
    // Corpus

    {"_init_corpus", 4, initCorpus_},
    {"_import_filter", 2, importFilter_},
    {"_import_pattern", 1, importPattern_},
    {"_import_train", 2, importTrain_},

    //
    //  Settings
    //  

    {"set_in_path", 4, set_in_path},
    {"set_bern", 1, set_bern},
    {"set_head_tail_cross_sampling", 1, set_head_tail_cross_sampling},
    {"set_work_threads", 1, set_work_threads},

    {"get_relation_total", 0, get_relation_total},
    {"get_entity_total", 0, get_entity_total},
    {"get_train_total", 0, get_train_total},
    {"get_test_total", 0, get_test_total},
    {"get_valid_total", 0, get_valid_total},
    
    //
    //  Random
    // 

    {"rand_reset", 0, rand_reset},

    // 
    //  Reader
    //  

    {"import_filter_patterns", 3, import_filter_patterns},

    {"import_train_files", 3, import_train_files},
    {"import_test_files", 3, import_test_files},
    {"import_type_files", 0, import_type_files},

    //
    //  Sampling
    //

    {"sample", 7, sample},
    // {"sample_symmetric", 7, sample_symmetric},

    //
    //  Testing
    //
    
    {"get_head_batch", 0, get_head_batch},
    {"test_head", 2, test_head},
    {"get_tail_batch", 0, get_tail_batch},
    {"test_tail", 2, test_tail},

    {"init_test", 0, init_test},

    //
    //  Validation
    //

    {"get_valid_head_batch", 0, get_valid_head_batch},
    {"valid_head", 2, valid_head},
    {"get_valid_tail_batch", 0, get_valid_tail_batch},
    {"valid_tail", 2, valid_tail},

    {"test_link_prediction", 2, test_link_prediction_}
};

ERL_NIF_INIT(Elixir.Grapex.Meager, meager_nif_funcs, NULL, NULL, NULL, NULL)

