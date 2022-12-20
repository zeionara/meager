#include "erl_nif.h"
// #include "utils.h"
#include "utils/nif/decode.h"
#include "utils/nif/encode.h"
#include "utils/nif/complete.h"
#include "../base/types.h"
// #include "../base/Test.h"

#include "../base/evaluation/link-prediction/Evaluator.h"
#include "../base/corpus/Default.h"
#include "../base/sampling/batch/Triple.h"

#include "../base/api/corpus.h"
#include "../base/api/evaluation.h"

#define evaluatorr evaluator

//
//  Test
//

// Evaluator<INT>* evaluatorr;
using namespace meager;

extern ERL_NIF_TERM
get_head_batch(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // cout << "get head batch start " << corpus->countEntities() << endl;
    int batch_tuple_size = main::api::corpus::corpus->countEntities();

    ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batch_tuple_size]();

    INT* batch_h_encoded = new INT[batch_tuple_size]();
    INT* batch_t_encoded = new INT[batch_tuple_size]();
    INT* batch_r_encoded = new INT[batch_tuple_size]();

    // getHeadBatch(batch_h_encoded, batch_t_encoded, batch_r_encoded);

    // cout << "Making head batch" << endl;

    sampling::batch::Triple* tripleBatch = main::api::evaluation::evaluatorr->head->makeBatch();

    // cout << "Made head batch" << endl;

    // cout << "Segmentation fault?" << endl;
    meager::erlang::utils::nif::encode::list(env, tripleBatch->head, batch_h, batch_tuple_size);
    // cout << "Segmentation fault?" << endl;
    meager::erlang::utils::nif::encode::list(env, tripleBatch->tail, batch_t, batch_tuple_size);
    meager::erlang::utils::nif::encode::list(env, tripleBatch->relation, batch_r, batch_tuple_size);
    // cout << "Segmentation fault?" << endl;

    // enif_encode_array_of_long(env, batch_h_encoded, batch_h, batch_tuple_size);
    // enif_encode_array_of_long(env, batch_t_encoded, batch_t, batch_tuple_size);
    // enif_encode_array_of_long(env, batch_r_encoded, batch_r, batch_tuple_size);

    delete [] batch_h_encoded;
    delete [] batch_t_encoded;
    delete [] batch_r_encoded;

    ERL_NIF_TERM* batch = new ERL_NIF_TERM[3]();

    batch[0] = enif_make_list_from_array(env, batch_h, batch_tuple_size);
    batch[1] = enif_make_list_from_array(env, batch_t, batch_tuple_size);
    batch[2] = enif_make_list_from_array(env, batch_r, batch_tuple_size);

    delete [] batch_h;
    delete [] batch_t;
    delete [] batch_r;

    ERL_NIF_TERM result = enif_make_list_from_array(env, batch, 3);

    delete [] batch;

    // cout << "get head batch end" << endl;
    return result;
}

extern ERL_NIF_TERM
test_head(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // cout << "test head batch start" << endl;
    int batch_tuple_size = main::api::corpus::corpus->countEntities();

    REAL* predictions_encoded = new REAL[batch_tuple_size]();

    meager::erlang::utils::nif::decode::list(env, argv[0], predictions_encoded, batch_tuple_size);
    
    // testHead(predictions_encoded, enif_get_bool(env, argv[1]));

    // cout << "Testing head" << endl;

    main::api::evaluation::evaluatorr->head->evaluate(predictions_encoded, meager::erlang::utils::nif::decode::boolean(env, argv[1]));

    // cout << "Tested head" << endl;

    // cout << "test head batch end" << endl;
    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
get_tail_batch(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // cout << "get tail batch start" << endl;
    int batch_tuple_size = main::api::corpus::corpus->countEntities();

    ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batch_tuple_size]();

    INT* batch_h_encoded = new INT[batch_tuple_size]();
    INT* batch_t_encoded = new INT[batch_tuple_size]();
    INT* batch_r_encoded = new INT[batch_tuple_size]();

    // getTailBatch(batch_h_encoded, batch_t_encoded, batch_r_encoded);

    // cout << "Making tail batch" << endl;

    sampling::batch::Triple* tripleBatch = main::api::evaluation::evaluatorr->tail->makeBatch();

    // cout << "Made tail batch" << endl;

    meager::erlang::utils::nif::encode::list(env, tripleBatch->head, batch_h, batch_tuple_size);
    meager::erlang::utils::nif::encode::list(env, tripleBatch->tail, batch_t, batch_tuple_size);
    meager::erlang::utils::nif::encode::list(env, tripleBatch->relation, batch_r, batch_tuple_size);

    // enif_encode_array_of_long(env, batch_h_encoded, batch_h, batch_tuple_size);
    // enif_encode_array_of_long(env, batch_t_encoded, batch_t, batch_tuple_size);
    // enif_encode_array_of_long(env, batch_r_encoded, batch_r, batch_tuple_size);

    delete [] batch_h_encoded;
    delete [] batch_t_encoded;
    delete [] batch_r_encoded;

    ERL_NIF_TERM* batch = new ERL_NIF_TERM[3]();

    batch[0] = enif_make_list_from_array(env, batch_h, batch_tuple_size);
    batch[1] = enif_make_list_from_array(env, batch_t, batch_tuple_size);
    batch[2] = enif_make_list_from_array(env, batch_r, batch_tuple_size);

    delete [] batch_h;
    delete [] batch_t;
    delete [] batch_r;

    ERL_NIF_TERM result = enif_make_list_from_array(env, batch, 3);

    delete [] batch;

    // cout << "get tail batch end" << endl;
    return result;
}

extern ERL_NIF_TERM
test_tail(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = main::api::corpus::corpus->countEntities();

    REAL* predictions_encoded = new REAL[batch_tuple_size]();

    meager::erlang::utils::nif::decode::list(env, argv[0], predictions_encoded, batch_tuple_size);

    // testTail(predictions_encoded, enif_get_bool(env, argv[1]));

    main::api::evaluation::evaluatorr->tail->evaluate(predictions_encoded, meager::erlang::utils::nif::decode::boolean(env, argv[1]));

    return enif_make_int(env, 0);
}

//
//  Validate
//


extern ERL_NIF_TERM
get_valid_head_batch(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = main::api::corpus::corpus->countEntities();

    ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batch_tuple_size]();

    INT* batch_h_encoded = new INT[batch_tuple_size]();
    INT* batch_t_encoded = new INT[batch_tuple_size]();
    INT* batch_r_encoded = new INT[batch_tuple_size]();

    // getValidHeadBatch(batch_h_encoded, batch_t_encoded, batch_r_encoded);

    meager::erlang::utils::nif::encode::list(env, batch_h_encoded, batch_h, batch_tuple_size);
    meager::erlang::utils::nif::encode::list(env, batch_t_encoded, batch_t, batch_tuple_size);
    meager::erlang::utils::nif::encode::list(env, batch_r_encoded, batch_r, batch_tuple_size);

    delete [] batch_h_encoded;
    delete [] batch_t_encoded;
    delete [] batch_r_encoded;

    ERL_NIF_TERM* batch = new ERL_NIF_TERM[3]();

    batch[0] = enif_make_list_from_array(env, batch_h, batch_tuple_size);
    batch[1] = enif_make_list_from_array(env, batch_t, batch_tuple_size);
    batch[2] = enif_make_list_from_array(env, batch_r, batch_tuple_size);

    delete [] batch_h;
    delete [] batch_t;
    delete [] batch_r;

    ERL_NIF_TERM result = enif_make_list_from_array(env, batch, 3);

    delete [] batch;

    return result;
}

extern ERL_NIF_TERM
valid_head(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = main::api::corpus::corpus->countEntities();

    REAL* predictions_encoded = new REAL[batch_tuple_size]();

    meager::erlang::utils::nif::decode::list(env, argv[0], predictions_encoded, batch_tuple_size);
    
    // validHead(predictions_encoded, enif_get_bool(env, argv[1]));

    return enif_make_int(env, 0);
}

extern ERL_NIF_TERM
get_valid_tail_batch(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = main::api::corpus::corpus->countEntities();

    ERL_NIF_TERM* batch_h = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_t = new ERL_NIF_TERM[batch_tuple_size]();
    ERL_NIF_TERM* batch_r = new ERL_NIF_TERM[batch_tuple_size]();

    INT* batch_h_encoded = new INT[batch_tuple_size]();
    INT* batch_t_encoded = new INT[batch_tuple_size]();
    INT* batch_r_encoded = new INT[batch_tuple_size]();

    // getValidTailBatch(batch_h_encoded, batch_t_encoded, batch_r_encoded);

    meager::erlang::utils::nif::encode::list(env, batch_h_encoded, batch_h, batch_tuple_size);
    meager::erlang::utils::nif::encode::list(env, batch_t_encoded, batch_t, batch_tuple_size);
    meager::erlang::utils::nif::encode::list(env, batch_r_encoded, batch_r, batch_tuple_size);

    delete [] batch_h_encoded;
    delete [] batch_t_encoded;
    delete [] batch_r_encoded;

    ERL_NIF_TERM* batch = new ERL_NIF_TERM[3]();

    batch[0] = enif_make_list_from_array(env, batch_h, batch_tuple_size);
    batch[1] = enif_make_list_from_array(env, batch_t, batch_tuple_size);
    batch[2] = enif_make_list_from_array(env, batch_r, batch_tuple_size);

    delete [] batch_h;
    delete [] batch_t;
    delete [] batch_r;

    ERL_NIF_TERM result = enif_make_list_from_array(env, batch, 3);

    delete [] batch;

    return result;
}

extern ERL_NIF_TERM
valid_tail(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    int batch_tuple_size = main::api::corpus::corpus->countEntities();

    REAL* predictions_encoded = new REAL[batch_tuple_size]();

    meager::erlang::utils::nif::decode::list(env, argv[0], predictions_encoded, batch_tuple_size);

    // validTail(predictions_encoded, enif_get_bool(env, argv[1]));

    return enif_make_int(env, 0);
}

ERL_NIF_TERM test_link_prediction_(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // test_link_prediction(
    //     enif_get_bool(env, argv[0], argv[1])
    // );

    main::api::evaluation::evaluatorr->printMetrics();

    return enif_make_int(env, 0);
}

ERL_NIF_TERM init_test(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    // initTest();

    // cout << "Initializing evaluatorr" << endl;

    // evaluatorr = new Evaluator<INT>(
    //     // new DefaultCorpus(trainList, testList, validList, types),
    //     corpus,
    //     corpus->train,
    //     corpus->test,
    //     []() {
    //         // INT const length = 7;
    //         INT const length = 7;
    //         INT i = 0;

    //         MetricTrackerBase** trackers = (MetricTrackerBase**) malloc(length * sizeof(MetricTrackerBase*));

    //         // trackers[i++] = new CountMetricTracker(1);
    //         // trackers[i++] = new CountMetricTracker(3);
    //         // trackers[i++] = new CountMetricTracker(10);
    //         // trackers[i++] = new RankMetricTracker();
    //         // trackers[i++] = new ReciprocalRankMetricTracker();

    //         trackers[i++] = new CountMetricTracker(1);
    //         trackers[i++] = new CountMetricTracker(3);
    //         trackers[i++] = new CountMetricTracker(10);
    //         trackers[i++] = new CountMetricTracker(100);
    //         trackers[i++] = new CountMetricTracker(1000);
    //         trackers[i++] = new RankMetricTracker();
    //         trackers[i++] = new ReciprocalRankMetricTracker();

    //         // cout << "Making new metric set tracker" << endl;

    //         // return new MetricSetTracker(trackers, length);

    //         // MetricTrackerBase (*trackerss)[length] = {
    //         // trackers = {
    //         //     new CountMetricTracker(1),
    //         //     new CountMetricTracker(3),
    //         //     new CountMetricTracker(10),
    //         //     new RankMetricTracker(),
    //         //     new ReciprocalRankMetricTracker()
    //         // };

    //         // return new MetricSetTracker();
    //         return new MetricSetTracker(trackers, length);
    //     }
    // );

    return enif_make_int(env, 0);
}

