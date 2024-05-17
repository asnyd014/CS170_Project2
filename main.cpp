/* References
https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c

*/
#include <iostream>
#include <random>
#include <ctime>

using namespace std;

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

}

void backwardElimination(int totalFeatures) {

}

double evaluation() {
    return uniform_rand(rand_engine);
}
