#include "Validator.h"

int main() {
    vector<vector<float>> trainingData;
    vector<float> featureSubset({3, 5, 7});
    // Read data from file into trainingData

    Classifer NN;
    Validator v(NN);
    v.leaveOneOutValidation(featureSubset, trainingData);

    return 0;
}
