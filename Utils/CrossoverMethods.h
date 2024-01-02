#ifndef PEA_PWR_3_CROSSOVERMETHODS_H
#define PEA_PWR_3_CROSSOVERMETHODS_H

#include <iostream>
#include <vector>
#include <set>
#include "../RandomDataGenerator/RandomDataGenerator.h"
#include "GASubject.h"

class CrossoverMethods {
public:
    static std::pair<std::vector<int>, std::vector<int>>
    OX(const std::vector<int> &first, const std::vector<int> &second, int matrixSize);
};


#endif //PEA_PWR_3_CROSSOVERMETHODS_H
