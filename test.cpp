#include "Validator.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

vector<vector<float>> readDataset(const string& filename) {
    vector<vector<float>> dataset;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return dataset;
    }

    string line;
    while (getline(file, line)) {
        istringstream lineStream(line);
        vector<float> instance;
        float value;

        while (lineStream >> value) {
            instance.push_back(value);
        }

        dataset.push_back(instance);
    }

    file.close();
    return dataset;
}

int main() {
    vector<vector<float>> trainingData;
    vector<int> featureSubset({1, 15, 27});
    // Read data from file into trainingData
    trainingData = readDataset("large-test-dataset-1.txt"); 
    cout.precision(4);

    cout << "Using features {";
    for (int i = 0; i < featureSubset.size(); ++i) {
        if (i != featureSubset.size()-1) { // Check if feature is last in vector
            cout << featureSubset[i] << ", ";
        }
        else {
            cout << featureSubset[i] << "}" << endl;
        }
    }

    Classifier NN;
    Validator v(NN);
    float acc = v.leaveOneOutValidation(featureSubset, trainingData);

    cout << "\nAccuracy = " << acc << endl;

    return 0;
}
