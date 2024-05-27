/*
 * Referenced for measuring execution time: https://stackoverflow.com/questions/22387586/measuring-execution-time-of-a-function-in-c
 */

#include "Classifier.h"
#include <algorithm>
#include <chrono>

using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

class Validator {
    Classifier NN;

    public:
    Validator(Classifier NN) {
        this->NN = NN;
    }

    // perform leave-one-out validation using specified feature subset and dataset
    float leaveOneOutValidation(vector<int> featureSubset, vector<vector<float>> dataset) {
        auto t1 = high_resolution_clock::now();
        auto t2 = high_resolution_clock::now();
        duration<double, milli> exec_time;
        t1 = high_resolution_clock::now();
        cout << "Filtering dataset for selected features..." << endl;
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
        t2 = high_resolution_clock::now();
        exec_time = t2 - t1;
        cout << "Filtering completed after " << exec_time.count() << "ms\n" << endl;
       
        // train the classifier with filtered dataset
        NN.train(filteredDataset);

        // leave-one-out validation
        int correctPredictions = 0;     // initialize counter for correct predictions
        for (int i = 0; i < dataset.size(); ++i) {           // iterate each instance in dataset
            t1 = high_resolution_clock::now();
            vector<float> testInstance = filteredDataset[i];    // get the test instance from filtered dataset
            int nearestNeighbor = NN.test(i);          // get the predicted class using trained classifier

            if (filteredDataset[nearestNeighbor][0] == dataset[i][0]) {      // compare predicted class with actual class(1st element of instance)
                correctPredictions++;                   // increment counter for correct predictions
            }

            t2 = high_resolution_clock::now();
            exec_time = t2 - t1;
            cout << "Instance " << i << " is class " << filteredDataset[i][0] << endl;
            cout << "Its nearest neighbor is " << nearestNeighbor << " which is in class " << filteredDataset[nearestNeighbor][0] << endl;
            cout << "Classification completed after " << exec_time.count() << "ms" << endl;
        }

        // calculate accuracy by dividing number of correct predictions by total number of instances
        float accuracy = static_cast<float>(correctPredictions) / dataset.size();
        return accuracy;
    }

};
