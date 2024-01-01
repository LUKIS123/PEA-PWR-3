#include "GASubject.h"

GASubject::GASubject(const std::vector<int> &path) : path(path) {
}

GASubject::GASubject(const std::vector<int> &firstParent, const std::vector<int> &secondParent) {

}

void GASubject::setPathCost(int **matrix) {
    auto currentV = path.begin();
    auto nextV = ++path.begin();
    int tmpCost = 0;

    for (; nextV != path.end(); currentV++) {
        tmpCost += matrix[*currentV][*nextV];
        nextV++;
    }
    pathCost = tmpCost;
}

void GASubject::setFitness(int populationTotalCosts, int populationSize) {

}
