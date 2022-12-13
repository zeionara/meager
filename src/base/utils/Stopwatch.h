#ifndef EVALUATION_STOPWATCH_H
#define EVALUATION_STOPWATCH_H

#include <stdexcept>
#include <iostream>
#include <chrono>

#define invalidArgument invalid_argument

using std::chrono::high_resolution_clock;
using std::chrono::system_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

using namespace std;

namespace meager::main::utils {

    struct Stopwatch {

        double* times;
        long length;

        // system_clock::time_point currentTime;
        system_clock::time_point currentTime;
        bool started;
        long i;

        Stopwatch(long length) {
            times = new double[length];
            started = false;
            i = -1;
        }

        void start() {
            currentTime = high_resolution_clock::now();
            started = true;
        }

        void stop() {
            if (!started) {
                throw invalidArgument("Cannot measure execution time because stopwatch was not started");
            }
            duration<double, std::milli> ms_duration = high_resolution_clock::now() - currentTime;
            times[++i] = (ms_duration).count();
            started = false;
        }

        void reset() {
            i = 0;
            started = false;
        }

        double mean() {
            return sum() / (i + 1);
        }

        double sum() {
            double sum = 0;
            for (long j = 0; j <= i; j++) {
                sum += times[j];
            }
            return sum;
        }

        void print(string label) {
            cout << label << " mean = " << mean() << " ms " << " total = " << sum() << " ms" << " (" << i << " measurements)" << endl;
        }

        ~Stopwatch() {
            delete [] times;
        }

    };

}

#endif
