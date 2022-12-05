#include "../base/evaluation/Stopwatch.h"

#include <iostream>

using namespace std;

int main() {
    auto stopwatch = Stopwatch(1);
    try {
        stopwatch.stop();
    } catch (invalidArgument&) {
        return 0;
    }
    return 1;
}
