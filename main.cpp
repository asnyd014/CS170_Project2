/* References
https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c

*/
#include <iostream>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

struct FeatureNode {
    vector<int> features;
    double accuracy;
};

void forwardSelection(int totalFeatures);
void backwardElimination(int totalFeatures);
double evaluation();

// Assuming accuracy will not be perfect, should be unnecessary in next part of project
uniform_real_distribution<double> uniform_rand(0.01, 99.99);
default_random_engine rand_engine;

int main() {
    rand_engine.seed(time(NULL)); // Seed random engine with time
    cout.precision(4); // Have cout ostream only display 4 digits of precision


    return 0;
}

void forwardSelection(int totalFeatures) {
    // Create list of feature values to be used for creation of node later
    vector<int> featureValueList;
    for (int i = 0; i < totalFeatures; ++i) {
        featureValueList.push_back(i+1);
    }

    // Create initial empty feature node
    FeatureNode initialNode;
    initialNode.accuracy = evaluation();

    // Nodes representing the tentative next choice and current maximum 
    FeatureNode nextNode = initialNode;
    FeatureNode maxNode = initialNode;

    vector<FeatureNode> nodeOptions;

    // For the number of total features, perform Forward Selection algorithm
    for (int i = 0; i < totalFeatures; ++i) {

        // Create combinations using features in the nextNode and featureValueList
        for (int j = 0; j < featureValueList.size(); ++j) {
            if (find(nextNode.features.begin(), nextNode.features.end(), featureValueList[j]) != nextNode.features.end()) { // Feature is not already in the nextNode's features
                FeatureNode newNode;
                vector<int> newFeatures = nextNode.features;
                newFeatures.push_back(j);
                newNode.features = newFeatures;
                newNode.accuracy = evaluation();
                nodeOptions.push_back(newNode);
            
                /*
                 * "Using features" output goes here, use information from newNode
                 */
            }
        }

        // Choose the next node from nodeOptions, update maxNode if a higher accuracy node is found
        nextNode = nodeOptions[0];
        for (int j = 0; j < nodeOptions.size(); ++j) {
            if (nodeOptions[j].accuracy > nextNode.accuracy) { nextNode = nodeOptions[j]; } // Choose new next node
            if (nodeOptions[j].accuracy > maxNode.accuracy) { maxNode = nodeOptions[j]; } // Update max node
        }

        /*
         * "Feature set was best" output goes here, use information from nextNode
         */
    }

    /*
     * "Finished search" output goes here, use information from maxNode
     */ 
}

void backwardElimination(int totalFeatures) {

}

double evaluation() {
    return uniform_rand(rand_engine);
}
