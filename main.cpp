/* References
https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c

*/
#include <string>
#include <fstream>
#include <sstream>
#include "Validator.h"

using namespace std;

struct FeatureNode {
    vector<int> features;
    float accuracy;
};

void forwardSelection(int totalFeatures, const vector<vector<float>> &dataset, Validator &v);
void backwardElimination(int totalFeatures, const vector<vector<float>> &dataset, Validator &v);
void outputFeatures(vector<int> features);
vector<vector<float>> readDataset(string filename);

int main() {
    int totalFeatures = 0;
    int algoNum = 0;
    string filename;
    vector<vector<float>> dataset;

    cout.precision(4);
    cout << "Welcome to Group 42's Feature Selection Algorithm" << endl;

    // Request name of dataset file
    cout << "\nType in the name of the file to test: ";
    cin >> filename;

    // Read in algorithm selection
    cout << "\nType the number of the algorithm you want to run:" << endl;
    cout << "1. Forward Selection" << "\n2. Backward Elimination" << endl;
    cin >> algoNum;

    dataset = readDataset(filename);
    totalFeatures = dataset[0].size() - 1;

    Classifier NN;
    Validator v(NN);

    switch(algoNum) {
        case 1:
            forwardSelection(totalFeatures, dataset, v);
            break;
        case 2:
            backwardElimination(totalFeatures, dataset, v);
            break;
        default:
            cout << "Invalid input" << endl;
    }

    return 0;
}

void forwardSelection(int totalFeatures, const vector<vector<float>> &dataset, Validator &v) {
    bool maxUpdated; // Used for determining if there has been a decrease in accuracy
    // Create list of feature values to be used for creation of node later
    vector<int> featureValueList;
    for (int i = 0; i < totalFeatures; ++i) {
        featureValueList.push_back(i+1);
    }

    // Create initial empty feature node
    FeatureNode initialNode;
    initialNode.accuracy = v.leaveOneOutValidation(initialNode.features, dataset);    

    // Output initial evaluation
    cout << "\nUsing no features, the current accuracy is  " << initialNode.accuracy << "%" << endl;
    cout << "\nBeginning Forward Selection search\n" << endl;

    // Nodes representing the tentative next choice and current maximum 
    FeatureNode nextNode = initialNode;
    FeatureNode maxNode = initialNode;

    vector<FeatureNode> nodeOptions;

    // For the number of total features, perform Forward Selection algorithm
    for (int i = 0; i < totalFeatures; ++i) {
        nodeOptions.clear(); // Destroy previous choices
        maxUpdated = false; // Initialize accuracy reduction indicator
        // Create combinations using features in the nextNode and featureValueList
        for (int j = 0; j < featureValueList.size(); ++j) {
            if (!nextNode.features.empty()) { // Check if this is the first selection       
                if (find(nextNode.features.begin(), nextNode.features.end(), featureValueList[j]) == nextNode.features.end()) { // Feature is not already in the nextNode's features
                    FeatureNode newNode;
                    vector<int> newFeatures = nextNode.features;
                    newFeatures.push_back(featureValueList[j]);
                    newNode.features = newFeatures;
                    newNode.accuracy = v.leaveOneOutValidation(newNode.features, dataset);
                    nodeOptions.push_back(newNode);

                    // Output feature information
                    cout << "\tUsing feature(s) ";
                    outputFeatures(newNode.features);
                    cout << "accuracy is " << newNode.accuracy << "%" << endl;
                }
            }
            else { // First selection
                FeatureNode newNode;
                vector<int> newFeatures;
                newFeatures.push_back(featureValueList[j]);
                newNode.features = newFeatures;
                newNode.accuracy = v.leaveOneOutValidation(newNode.features, dataset);
                nodeOptions.push_back(newNode);

                // Output feature information
                cout << "\tUsing feature(s) ";
                outputFeatures(newNode.features);
                cout << "accuracy is " << newNode.accuracy << "%" << endl; 
            }
        }

        // Choose the next node from nodeOptions, update maxNode if a higher accuracy node is found
        nextNode = nodeOptions[0];
        for (int j = 0; j < nodeOptions.size(); ++j) {
            if (nodeOptions[j].accuracy > nextNode.accuracy) { nextNode = nodeOptions[j]; } // Choose new next node
            if (nodeOptions[j].accuracy > maxNode.accuracy) { maxNode = nodeOptions[j]; maxUpdated = true; } // Update max node
        }

        if (i != featureValueList.size()-1) { // Do not output nextNode info if this is the last selection
            // Output choice of next node
            cout << "\nFeature set ";
            outputFeatures(nextNode.features);
            cout << "was best, accuracy is " << nextNode.accuracy << "%" << endl;
            if (!maxUpdated) {
                cout << "Warning: Accuracy has decreased from previously recorded maximum\n" << endl;
            }
            else {
                cout << endl;
            }
        }
        else if (!maxUpdated) {
            cout << "\nWarning: Accuracy has decreased from previously recorded maximum\n" << endl;
        }
    }

    // Output final best choice based on maxNode
    cout << "\nFinished search. The best feature subset is ";
    outputFeatures(maxNode.features);
    cout << "which has an accuracy of " << maxNode.accuracy << "%" << endl;
}

void backwardElimination(int totalFeatures, const vector<vector<float>> &dataset, Validator &v) {
    bool maxUpdated; // Used for determining if there has been a decrease in accuracy
    // Create list of feature values to be used for creation of node later
    vector<int> featureValueList;
    FeatureNode initialNode;
    for (int i = 0; i < totalFeatures; ++i) {
        featureValueList.push_back(i+1);
        initialNode.features.push_back(i+1); // Push all features to initial node as well
    }

    // Create initial empty feature node
    initialNode.accuracy = v.leaveOneOutValidation(initialNode.features, dataset);

    // Output initial evaluation
    cout << "\nUsing features ";
    outputFeatures(initialNode.features);
    cout << " the current accuracy is " << initialNode.accuracy << "%" << endl;
    cout << "\nBeginning Backward Elimination search\n" << endl;

    // Nodes representing the tentative next choice and current maximum 
    FeatureNode nextNode = initialNode;
    FeatureNode maxNode = initialNode;

    vector<FeatureNode> nodeOptions;

    // For the number of total features, perform Forward Selection algorithm
    for (int i = 0; i < totalFeatures; ++i) {
        nodeOptions.clear(); // Destroy previous choices
        maxUpdated = false; // Initialize accuracy reduction indicator
        // Create combinations using features in the nextNode and featureValueList
        for (int j = 0; j < featureValueList.size(); ++j) {
            if (nextNode.features.size() != 1) { // Check if this is the last selection     
                if (find(nextNode.features.begin(), nextNode.features.end(), featureValueList[j]) != nextNode.features.end()) { // Feature is still in the nextNode's features
                    FeatureNode newNode;
                    vector<int> newFeatures = nextNode.features;
                    newFeatures.erase(find(newFeatures.begin(), newFeatures.end(), featureValueList[j])); // Remove element from features
                    newNode.features = newFeatures;
                    newNode.accuracy = v.leaveOneOutValidation(newNode.features, dataset);
                    nodeOptions.push_back(newNode);

                    // Output feature information
                    cout << "\tUsing feature(s) ";
                    outputFeatures(newNode.features);
                    cout << "accuracy is " << newNode.accuracy << "%" << endl;
                }
            }
            else { // Last selection, features vector is empty
                FeatureNode newNode;
                newNode.accuracy = v.leaveOneOutValidation(newNode.features, dataset);
                nodeOptions.push_back(newNode);

                // Output feature information
                cout << "\tUsing no features, accuracy is " << newNode.accuracy << "%" << endl; 
                break;
            }
        }

        // Choose the next node from nodeOptions, update maxNode if a higher accuracy node is found
        nextNode = nodeOptions[0];
        for (int j = 0; j < nodeOptions.size(); ++j) {
            if (nodeOptions[j].accuracy > nextNode.accuracy) { nextNode = nodeOptions[j]; } // Choose new next node
            if (nodeOptions[j].accuracy > maxNode.accuracy) { maxNode = nodeOptions[j]; maxUpdated = true; } // Update max node
        }

        if (i != featureValueList.size()-1) { // Do not output nextNode info if this is the last selection
            // Output choice of next node
            cout << "\nFeature set ";
            outputFeatures(nextNode.features);
            cout << "was best, accuracy is " << nextNode.accuracy << "%" << endl;
            if (!maxUpdated) {
                cout << "Warning: Accuracy has decreased from previously recorded maximum\n" << endl;
            }
            else {
                cout << endl;
            }
        }
        else if (!maxUpdated) {
            cout << "\nWarning: Accuracy has decreased from previously recorded maximum\n" << endl;
        }
    }

    // Output final best choice based on maxNode
    cout << "\nFinished search. The best feature subset is ";
    outputFeatures(maxNode.features);
    cout << "which has an accuracy of " << maxNode.accuracy << "%" << endl;
}

void outputFeatures(vector<int> features) {
    int size = features.size();
    cout << "{";
    if (!features.empty()) { // Check if features vector is empty   
        for (int i = 0; i < size; ++i) {
            if (i != size-1) { // Check if number is the last in the vector
                cout << features[i] << ",";
            }
            else { cout << features[i] << "} "; }
        }
    }
    else { cout << "} "; }
}

vector<vector<float>> readDataset(string filename) {
    vector<vector<float>> dataset;
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        exit(0);
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
