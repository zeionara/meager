#include <string>

using namespace std;

namespace meager::main::evaluation::task {

    enum class Task {
        LinkPrediction,
        TripleClassification
    };

    Task decodeName(string task);

}
