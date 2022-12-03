#include <string>

using namespace std;

enum EvaluationTask { LINK_PREDICTION, TRIPLE_CLASSIFICATION };

EvaluationTask decodeEvaluationTask(string task);
