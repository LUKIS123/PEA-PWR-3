#ifndef PEA_PWR_3_GREEDYALGORITHM_H
#define PEA_PWR_3_GREEDYALGORITHM_H

#include <vector>
#include <iostream>
#include <algorithm>

class GreedyAlgorithm {
public:
    GreedyAlgorithm() = default;

    virtual ~GreedyAlgorithm() = default;

    int bestCost = INT_MAX;
    std::vector<int> bestPath;

    static std::pair<std::vector<int>, int> solveGreedyAlgorithm(int **matrix, int matrixSize, int startVertex);

    std::vector<std::pair<std::vector<int>, int>> getBestGreedyAlgorithmResult(int **matrix, int matrixSize);
};


#endif //PEA_PWR_3_GREEDYALGORITHM_H
