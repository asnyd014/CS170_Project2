#include "Classifier.h"
#include <algorithm>

class Validator {
    Classifier NN;

    public:
    Validator(Classifier NN) {
        this->NN = NN;
    }

    // perform leave-one-out validation using specified feature subset and dataset
    float leaveOneOutValidation(vector<int> featureSubset, vector<vector<float>> dataset) {
        vector<vector<float>> filteredDataset;      // initialize vector to store dataset w/ filtered features
        for (const auto& instance : dataset) {      // iterate each instance in dataset
            vector<float> extractedFeatures;        // initialize vector to store extracted features for each instance
            extractedFeatures.push_back(instance[0]);
            for (size_t i = 0; i < instance.size(); ++i) {      // iterate each feature in instance
                if (find(featureSubset.begin(), featureSubset.end(), i) != featureSubset.end()) {   // check if feature index is in the specified feature subset
                    extractedFeatures.push_back(instance[i]);   // add the feature to extracted features
                }
            }
            filteredDataset.push_back(extractedFeatures);   // add the extracted features to filtered dataset
        }
       
        // train the classifier with filtered dataset
        NN.train(filteredDataset);

        // leave-one-out validation
        int correctPredictions = 0;     // initialize counter for correct predictions
        for (size_t i = 0; i < dataset.size(); ++i) {           // iterate each instance in dataset
            vector<float> testInstance = filteredDataset[i];    // get the test instance from filtered dataset
            float predictedClass = NN.test(i);          // get the predicted class using trained classifier

            if (predictedClass == dataset[i][0]) {      // compare predicted class with actual class(1st element of instance)
                correctPredictions++;                   // increment counter for correct predictions
            }
        }

        // calculate accuracy by dividing number of correct predictions by total number of instances
        float accuracy = static_cast<float>(correctPredictions) / dataset.size();
        return accuracy;
    }

};
