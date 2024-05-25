#include <vector>

class Classifier {
    vector<vector<float>> trainingInstances;

    public:
    void train(vector<vector<float>> trainingData) {
        trainingInstances = trainingData;
    }

    float test(int testID) {
        // Calculate Euclidean distance to all trainingInstances to determine class
    }
}
