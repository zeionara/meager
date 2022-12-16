#ifndef UTILS_DECODE_METRIC_H
#define UTILS_DECODE_METRIC_H

// #include "../../../base/sampling/batch/Triple.h"
#include "../../../base/evaluation/metric/main.h"

#include "../../../base/evaluation/metric/tracker/Count.h"
#include "../../../base/evaluation/metric/tracker/Rank.h"
#include "../../../base/evaluation/metric/tracker/ReciprocalRank.h"

#include "../main.h"

namespace meager::python::utils::decode::metric {

    template <typename T>
    List<main::evaluation::metric::tracker::TrackerBase*>* metrics(char* metrics) {
        unsigned char length = metrics[0];
        unsigned char i = 0;
        unsigned char offset = 1;  // The list starts from the 1st byte
        
        main::evaluation::metric::tracker::TrackerBase** result = new main::evaluation::metric::tracker::TrackerBase*[length];

        while (i < length) {
            unsigned char nParams = metrics[offset++];

            std::string metricName = &metrics[offset];
            offset += metricName.length() + 1;

            T* params = (T*)&(metrics[offset]);

            switch (main::evaluation::metric::decodeName(metricName)) {
                case main::evaluation::metric::Metric::Count:
                    if (nParams > 0) {
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
                    throw invalidArgument("Unspecified metric tracker for metric " + std::string(metricName));

            }

            offset += sizeof(T) * nParams;

        }

        return new List(result, length);
    }

}

#endif
