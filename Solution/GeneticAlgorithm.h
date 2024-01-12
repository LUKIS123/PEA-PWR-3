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
#include "../Flow/Enums.h"

class GeneticAlgorithm {
private:
    bool isRandomPopulationInitialization = false;
    CrossMethod crossoverMethod = CrossMethod::OX;
    GreedyAlgorithm *greedyAlgorithm;

    int **matrix = nullptr;
    int matrixSize;
    int populationSize;
    double mutationFactor = 0.01;
    double crossFactor = 0.8;

    int tournamentParticipants;

    double eliteFactor = 1.0 - crossFactor;
    int eliteCount = 0;
    std::list<GASubject> currentElite;


    std::vector<GASubject> matingPool;
    int matingPoolSize;

    int mutationPoolSize;

    std::vector<GASubject> currentPopulation;
    std::vector<GASubject> nextPopulation;

    int timeoutSeconds = 120;
    long long int startQPC;

public:
    long long int bestCostFoundQPC;
    std::vector<int> bestPath;
    int bestCost = INT_MAX;

    int startingPopulationBestPathCost;

    // TESTING
    bool testing = false;
    std::vector<double> timestamps;
    std::vector<int> solutionProgressionPoints;
    //


    void clearMemory();

    void displayLatestResults();

    GeneticAlgorithm();

    virtual ~GeneticAlgorithm();

    void mainFun(int **matrix, int matrixSize, int populationSize, double mutationFactor, double crossFactor,
                 long long int startQPC, int timeout, bool isRandomInit, CrossMethod method);

    void solveTSP();

    void initializePopulation();

    void initializePopulationWithRandomPaths();

    std::pair<GASubject, GASubject>
    tournamentSelection(std::uniform_int_distribution<> &distInt, std::mt19937 &device);

    std::pair<GASubject, GASubject> crossSubjects(GASubject &first, GASubject &second) const;

    void mutate(GASubject &subject) const;

private:
    static bool inRange(unsigned int low, unsigned int high, unsigned int x) {
        return ((x - low) <= (high - low));
    }

};


#endif //PEA_PWR_3_GENETICALGORITHM_H
