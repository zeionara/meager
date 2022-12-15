#include "../base/api/evaluation.h"
#include "../base/evaluation/metric/main.h"

#include "../base/evaluation/metric/tracker/Count.h"
#include "../base/evaluation/metric/tracker/Rank.h"
#include "../base/evaluation/metric/tracker/ReciprocalRank.h"

namespace meager::python::utils::decode {

    template <typename T>
    struct List {
        T* items;
        long length;

        List(T* items, long length) {
            this->items = items;
            this->length = length;
        }

        ~List() {
            delete [] items;

        }

    };

    List<meager::main::evaluation::metric::tracker::TrackerBase*>* metrics(char* metrics);

}

extern "C" void meager__python__api__evaluation__init(char* metrics, char* taskName, char* subsetName, bool verbose) {
    evaluation::task::Task task = meager::main::evaluation::task::decodeName(taskName);
    subset::Type subset = subset::decodeName(subsetName);

    meager::main::api::evaluation::init([metrics](string label){
        // utils::nif::decode::List<main::evaluation::metric::tracker::TrackerBase*>* trackers = utils::nif::decode::list<main::evaluation::metric::tracker::TrackerBase*>(
        //     env, argv[0], [](ErlNifEnv* env, ERL_NIF_TERM metric
        // ) -> main::evaluation::metric::tracker::TrackerBase* {
        //     int length;
        //     const ERL_NIF_TERM* metricDescription;

        //     enif_get_tuple(env, metric, &length, &metricDescription);

        //     if (length < 1) {
        //         throw invalidArgument("Empty metric descriptions are not allowed");
        //     }

        //     char* metricName = utils::nif::decode::atom(env, metricDescription[0]);

        //     switch (main::evaluation::metric::decodeName(metricName)) {
        //         case main::evaluation::metric::Metric::Count:
        //             if (length > 1)
        //                 return new main::evaluation::metric::tracker::Count(utils::nif::decode::longInteger(env, metricDescription[1])); 
        //             throw invalidArgument("Metric count requires one parameter (top-n)");
        //         case main::evaluation::metric::Metric::Rank:
        //             if (length < 2)
        //                 return new main::evaluation::metric::tracker::Rank(); 
        //             throw invalidArgument("Metric rank doesn't accept parameters");
        //         case main::evaluation::metric::Metric::ReciprocalRank:
        //             if (length == 1)
        //                 return new main::evaluation::metric::tracker::ReciprocalRank(); 
        //             throw invalidArgument("Metric reciprocal-rank doesn't accept parameters");
        //         default:
        //             throw invalidArgument("Unspecified metric tracker for metric " + string(metricName));
        //     }
        // });

        // return new main::evaluation::metric::tracker::Set(trackers->items, trackers->length, label);
        auto decodedMetrics = meager::python::utils::decode::metrics(metrics);
        return new meager::main::evaluation::metric::tracker::Set(decodedMetrics->items, decodedMetrics->length, label);
    }, task, subset, verbose);
}

#define PARAM unsigned short


namespace meager::python::utils::decode {

    List<meager::main::evaluation::metric::tracker::TrackerBase*>* metrics(char* metrics) {
        unsigned char length = metrics[0];
        unsigned char i = 0;
        unsigned char offset = 1;  // The list starts from the 1st byte
        
        meager::main::evaluation::metric::tracker::TrackerBase** result = new meager::main::evaluation::metric::tracker::TrackerBase*[length];

        // printf("number of metrics = %i\n", length);
        // cout << "number of metrics = " << length << endl;

        while (i < length) {
            unsigned char nParams = metrics[offset++];
            // cout << sizeof(PARAM) << endl;
            // printf("number of params for %i metric = %i\n", i, nParams);
            string metricName = &metrics[offset];
            // cout << metricName << endl;
            offset += metricName.length() + 1;

            PARAM* params = (PARAM*)&(metrics[offset]);
            // cout << "accessing params" << endl;
            // cout << params[0] << endl;
            // printf("byte at offset = %i\n", metrics[offset]);
            // printf("next byte at offset = %i\n", metrics[offset + 1]);
            // printf("address of this value = %d", &metrics[offset]);
            // cout << params << endl;
            // cout << endl;

            switch (main::evaluation::metric::decodeName(metricName)) {
                case main::evaluation::metric::Metric::Count:
                    if (nParams > 0) {
                        // cout << "initialize metric count with parameter = " << params[0] << endl;
                        result[i++] = new main::evaluation::metric::tracker::Count(params[0]); 
                        break;
                    }
                    throw invalidArgument("Metric count requires one parameter (top-n)");
                case main::evaluation::metric::Metric::Rank:
                    if (nParams < 1) {
                        result[i++] = new main::evaluation::metric::tracker::Rank(); 
                        break;
                    }
                    throw invalidArgument("Metric rank doesn't accept parameters");
                case main::evaluation::metric::Metric::ReciprocalRank:
                    if (nParams < 1) {
                        result[i++] = new main::evaluation::metric::tracker::ReciprocalRank(); 
                        break;
                    }
                    throw invalidArgument("Metric reciprocal-rank doesn't accept parameters");
                default:
                    throw invalidArgument("Unspecified metric tracker for metric " + string(metricName));

            }

            offset += sizeof(PARAM) * nParams;

        }

        // cout << "hooray" << endl;

        return new List(result, length);
    }

}
