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
#include "../Utils/CrossoverMethods.h"

class GeneticAlgorithm {
private:
    int **matrix = nullptr;
    int matrixSize;
    int populationSize;
    double mutationFactor = 0.01;
    double crossFactor = 0.8;

    int tournamentParticipants;

    std::vector<GASubject> currentPopulation;
    std::vector<GASubject> nextPopulation;

    int timeoutSeconds = 120;
    long long int startQPC;

public:
    long long int bestCostFoundQPC;
    std::vector<int> bestPath;
    int bestCost = INT_MAX;

    int greedyAlgorithmCost;

    // TESTING
    bool testing = false;
    std::vector<double> timestamps;
    std::vector<int> solutionProgressionPoints;
    //


    void clearMemory();

    void displayLatestResults();

    GeneticAlgorithm() = default;

    virtual ~GeneticAlgorithm();

    void mainFun(int **matrix, int matrixSize, int populationSize, double mutationFactor, double crossFactor,
                 long long int startQPC, int timeout);

    void solveTSP();

    void initializePopulation();

    std::pair<GASubject, GASubject> tournamentSelection(int randomIndex);

    std::pair<GASubject, GASubject> crossSubjects(GASubject &first, GASubject &second) const;

    void mutate(GASubject &subject) const;

    void insertOffspring(GASubject &subject);
};


#endif //PEA_PWR_3_GENETICALGORITHM_H
