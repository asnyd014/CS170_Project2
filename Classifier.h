#include <vector>
#include <cmath>
#include <climits>
#include <iostream>

using namespace std;

class Classifier {
    vector<vector<float>> trainingInstances;

    public:
    void train(vector<vector<float>> trainingData) {
        trainingInstances = trainingData;
    }

    float test(int testID) {
        float minDistance = INT_MAX;
        int minIndex = 0;
        float tempDistance = 0;
        // Calculate Euclidean distance to all trainingInstances to determine class
        for (int i = 0; i < trainingInstances.size(); ++i) {
            if (i != testID) { // Avoid checking distance to itself
                tempDistance = 0;
                for (int j = 1; j < trainingInstances[0].size(); ++j) { // Iterate through provided features only, index will be the same between instances
                    tempDistance += pow((trainingInstances[i][j] - trainingInstances[testID][j]), 2);
                }
                tempDistance = sqrt(tempDistance);
                
                // Update minimum if applicable
                if (tempDistance < minDistance) {
                    minDistance = tempDistance;
                    minIndex = i;
                }
            }
        }

        // Return class of instance at minIndex
        return trainingInstances[minIndex][0];
    }
};
