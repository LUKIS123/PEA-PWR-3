#include "GeneticAlgorithm.h"

GeneticAlgorithm::~GeneticAlgorithm() {

}

void GeneticAlgorithm::displayLatestResults() {
    std::cout << "ALGORITHM RESULTS:" << std::endl;
    std::cout << "Path: ";
    for (const auto &item: bestPath) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
    std::cout << "Best Cost: " << bestPathCost << std::endl;
}

void
GeneticAlgorithm::mainFun(int **matrix, int matrixSize, int populationSize, double mutationFactor, double crossFactor,
                          long long int startQPC, int timeout) {
    this->matrix = matrix;
    this->matrixSize = matrixSize;
    this->populationSize = populationSize;
    this->mutationFactor = mutationFactor;
    this->crossFactor = crossFactor;
    this->startQPC = startQPC;
    this->timeoutSeconds = timeout;

    currentPopulation.reserve(populationSize);
    nextPopulation.reserve(populationSize);

    solveTSP();
}

void GeneticAlgorithm::solveTSP() {
    std::chrono::time_point breakAlgoTimePoint = std::chrono::system_clock::now() +
                                                 std::chrono::duration_cast<std::chrono::seconds>(
                                                         std::chrono::duration<int>(timeoutSeconds)
                                                 );
    initializePopulation();

    for (int iteration = 0; true; ++iteration) {
        // Przerwanie algorytmu jesli osiagnieto kryterium stopu
        if ((breakAlgoTimePoint - std::chrono::system_clock::now()).count() < 0) {
            break;
        }

        // todo: liczenie fitness i selekcja
        for (int j = 0; j < matrixSize; ++j) {
            currentPopulation[j].setFitness(currentPopulationTotalCost, populationSize);
            // ewaluacja jesli fit > 1 i napisac metode obliczajaca fitness
            // todo: tu juz mozna dodawac do nastepnej populacji
        }

    }

}

void GeneticAlgorithm::initializePopulation() {
    // Wyznaczanie rozwiazania przy pomocy metody zachlannej
    auto pathCostPair = GreedyAlgorithm::getBestGreedyAlgorithmResult(matrix, matrixSize);
    greedyAlgorithmCost = pathCostPair.second;

    int totalPopulationCost = 0;
    for (int i = 0; i < populationSize; ++i) {
        std::vector<int> path(pathCostPair.first);
        path.pop_back();

        int v1 = RandomDataGenerator::generateVertexInRange(0, matrixSize - 1);
        int v2;
        do {
            v2 = RandomDataGenerator::generateVertexInRange(0, matrixSize - 1);
        } while (v2 == v1);

        int randomPerturbationOption = RandomDataGenerator::generateVertexInRange(0, 2);

        if (randomPerturbationOption == 0) {
            std::swap(path[v1], path[v2]);
        } else if (randomPerturbationOption == 1) {
            std::reverse(path.begin() + v1, path.begin() + v2);
        } else {
            v1 = RandomDataGenerator::generateVertexInRange(0, matrixSize - 1);
            do {
                v2 = RandomDataGenerator::generateVertexInRange(0, matrixSize - 1);
            } while (v2 == v1 || abs((v2 - v1)) > (matrixSize / 2));
            if (v1 > v2) {
                std::swap(v1, v2);
            }
            std::random_device rdev;
            std::mt19937 gen(rdev());
            std::shuffle(path.begin() + v1, path.begin() + v2, gen);
        }

        path.push_back(path[0]);
        auto subject = GASubject(path);
        subject.setPathCost(matrix);
        totalPopulationCost += subject.pathCost;

        currentPopulation.push_back(subject);
    }
    currentPopulationTotalCost = totalPopulationCost;
}
