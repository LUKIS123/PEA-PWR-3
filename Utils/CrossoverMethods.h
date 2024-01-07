#ifndef PEA_PWR_3_CROSSOVERMETHODS_H
#define PEA_PWR_3_CROSSOVERMETHODS_H

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include "../RandomDataGenerator/RandomDataGenerator.h"
#include "GASubject.h"

class CrossoverMethods {
public:
    static std::pair<std::vector<int>, std::vector<int>>
    orderCrossover(const std::vector<int> &first, const std::vector<int> &second, int matrixSize);

    static std::vector<int>
    edgeCrossover(const std::vector<int> &first, const std::vector<int> &second, int matrixSize);

    static std::_List_iterator<std::pair<int, bool>>
    findElementOfEdgeList(std::list<std::pair<int, bool>> &edgeList, int value);

    static void addOffspring(std::list<int> &offspring, int vertex, bool direction);

    static std::pair<bool, int> tryAddBestNeighbor(int vertex, bool direction, std::list<int> &offspring,
                                                   std::vector<std::list<std::pair<int, bool>>> &edgeTab);

    static void removeRemaining(int vertex, std::list<int> &rem);
};


#endif //PEA_PWR_3_CROSSOVERMETHODS_H
