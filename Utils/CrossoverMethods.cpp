#include "CrossoverMethods.h"

std::pair<std::vector<int>, std::vector<int>>
CrossoverMethods::OX(const std::vector<int> &first, const std::vector<int> &second, int matrixSize) {
    std::vector<int> parent1 = first;
    parent1.pop_back();
    std::vector<int> parent2 = second;
    parent2.pop_back();

    int v1 = RandomDataGenerator::generateRandomIntInRange(0, matrixSize - 1);
    int v2;
    do {
        v2 = RandomDataGenerator::generateRandomIntInRange(0, matrixSize - 1);
    } while (v2 == v1);

    if (v1 > v2) {
        std::swap(v1, v2);
    }

    std::vector<int> offspring1(matrixSize);
    std::set<int> offspring1Vertexes;

    std::vector<int> offspring2(matrixSize);
    std::set<int> offspring2Vertexes;

    for (int i = v1; i <= v2; ++i) {
        offspring1[i] = parent1[i];
        offspring1Vertexes.insert(parent1[i]);

        offspring2[i] = parent2[i];
        offspring2Vertexes.insert(parent2[i]);
    }

    int currentOffspring1Index = (v2 + 1) % matrixSize;
    int currentOffspring2Index = currentOffspring1Index;
    int limit = currentOffspring1Index;

//    for (int j = currentOffspring1Index; j != v2; j = (j + 1) % matrixSize) {
//        if (offspring1Vertexes.find(parent2[j]) == offspring1Vertexes.end()) {
//            offspring1[currentOffspring1Index] = parent2[j];
//            currentOffspring1Index = (currentOffspring1Index + 1) % matrixSize;
//        }
//        if (offspring2Vertexes.find(parent1[j]) == offspring2Vertexes.end()) {
//            offspring2[currentOffspring2Index] = parent1[j];
//            currentOffspring2Index = (currentOffspring2Index + 1) % matrixSize;
//        }
//    }

    int index = currentOffspring1Index;
    do {
        if (offspring1Vertexes.find(parent2[index]) == offspring1Vertexes.end()) {
            offspring1[currentOffspring1Index] = parent2[index];
            currentOffspring1Index = (currentOffspring1Index + 1) % matrixSize;
        }
        if (offspring2Vertexes.find(parent1[index]) == offspring2Vertexes.end()) {
            offspring2[currentOffspring2Index] = parent1[index];
            currentOffspring2Index = (currentOffspring2Index + 1) % matrixSize;
        }

        index = (index + 1) % matrixSize;
    } while (index != limit);

    offspring1.push_back(offspring1[0]);
    offspring2.push_back(offspring2[0]);
    return std::make_pair(offspring1, offspring2);
}
