#ifndef PEA_PWR_3_APPCONTROLLER_H
#define PEA_PWR_3_APPCONTROLLER_H

#include <iostream>
#include <limits>
#include <cmath>
#include "./ActionResult.h"
#include "./ConsoleView.h"
#include "./Enums.h"
#include "../Matrix/ATSPMatrix.h"
#include "../MeasureTime/Timer.h"
#include "../FileUtils/DataFileUtility.h"
#include "../Solution/GeneticAlgorithm.h"

class AppController {
private:
    ATSPMatrix *matrix = nullptr;
    GeneticAlgorithm *geneticAlgorithm = nullptr;

    std::string algorithmTypes[2] = {
            "GENETIC"
    };
    std::string resultPathFileName = "latest_result_path";
    int testNumber = 10;

    LatestAlgorithm latestRun = LatestAlgorithm::NONE;
    long long int latestTimerStart = 0;
    double latestTimerResult = 0.0;

    int timeoutSeconds = 120;

    // GENETIC ALGORITHM PARAMS
    bool isRandomPathPopulationInitialization = true;
    CrossMethod crossoverMethod = CrossMethod::OX;
    int populationSize = 250;
    double mutationFactor = 0.01;
    double crossFactor = 0.8;

public:
    AppController();

    virtual ~AppController();

    void clearMemory();

    void mainIndex();

    void loadDataFile();

    void setTimeout();

    void testsMenu();

    void displayLatestResults();

    void readPathAndDisplayCalculatedCost();

    void testGeneticAlgorithm(bool crossMethod);

    void runGeneticAlgorithm();

    void setPopulationSize();

    void setMutationFactor();

    void setCrossFactor();

    void switchCrossMethod();
};


#endif //PEA_PWR_3_APPCONTROLLER_H
