// #include "../base/api/evaluation.h"
#include "../base/sampling/batch/Triple.h"
#include "../base/evaluation/metric/main.h"

#include "../base/evaluation/metric/tracker/Count.h"
#include "../base/evaluation/metric/tracker/Rank.h"
#include "../base/evaluation/metric/tracker/ReciprocalRank.h"

namespace meager::python::utils {

    namespace encode {

        void** tripleBatch(meager::main::sampling::batch::Triple* batch, bool includeLabels = true);

    }

    namespace decode {
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

        template <typename T>
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

                T* params = (T*)&(metrics[offset]);
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

                offset += sizeof(T) * nParams;

            }

            // cout << "hooray" << endl;

            return new List(result, length);
        }

    }

}
