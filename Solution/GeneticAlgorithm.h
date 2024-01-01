#ifndef PEA_PWR_3_GENETICALGORITHM_H
#define PEA_PWR_3_GENETICALGORITHM_H

#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <utility>
#include <cmath>
#include <chrono>
#include <random>
#include "../Matrix/ATSPMatrix.h"
#include "../RandomDataGenerator/RandomDataGenerator.h"
#include "../MeasureTime/Timer.h"
#include "../Utils/GreedyAlgorithm.h"
#include "../Utils/GASubject.h"

class GeneticAlgorithm {
private:
    int **matrix = nullptr;
    int matrixSize;
    int populationSize;
    double mutationFactor = 0.8;
    double crossFactor = 0.01;

    std::vector<GASubject> currentPopulation;
    int currentPopulationTotalCost = 0;
    std::vector<GASubject> nextPopulation;
    int nextPopulationTotalCost = 0;

    int timeoutSeconds = 120;
    int bestPathCost = INT_MAX;
    long long int startQPC;

public:
    long long int bestCostFoundQPC;
    const std::vector<int> bestPath;

    // TESTING
    bool testing = false;
    std::vector<double> timestamps;
    std::vector<int> solutionProgressionPoints;
    //
    int greedyAlgorithmCost;


    void displayLatestResults();

    GeneticAlgorithm() = default;

    virtual ~GeneticAlgorithm();

    void mainFun(int **matrix, int matrixSize, int populationSize, double mutationFactor, double crossFactor,
                 long long int startQPC, int timeout);

    void solveTSP();

    void initializePopulation();
};


#endif //PEA_PWR_3_GENETICALGORITHM_H
