#include "Classifier.h"

class Validator {
    Classifier NN;

    public:
    Validator(Classifier NN) {
        this->NN = NN;
    }

    float leaveOneOutValidation(vector<float> featureSubset, vector<vector<float>> dataset) {
        /*  Create a new vector<vector<float>> containing only the features specified in the featureSubset
            Feed this new vector<vector<float>> to the Classifier's train() function (will overwrite its stored "training data")
            For each vector in the new vector<vector<float>>,
            - Feed vector<float> to Classifier's test function
            - If returned float matches the class provided (first index of vector<float>), incremement an integer representing successful classifications
            Return calculated accuracy ( successes / size of dataset )
        */
    }
}
