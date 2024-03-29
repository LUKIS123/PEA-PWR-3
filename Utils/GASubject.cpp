#include "GASubject.h"

GASubject::GASubject() = default;

GASubject::GASubject(const std::vector<int> &path) : path(path) {
}

GASubject::GASubject(const std::vector<int> &path, int pathCost) : path(path), pathCost(pathCost) {
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
