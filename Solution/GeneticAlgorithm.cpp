#include "GeneticAlgorithm.h"


GeneticAlgorithm::GeneticAlgorithm() {
    greedyAlgorithm = new GreedyAlgorithm();
}

GeneticAlgorithm::~GeneticAlgorithm() {
    delete greedyAlgorithm;
}

void GeneticAlgorithm::clearMemory() {
    if (testing) {
        timestamps.clear();
        solutionProgressionPoints.clear();
    }
    bestCostFoundQPC = Timer::read_QPC();
    bestPath.clear();
    bestCost = INT_MAX;
    currentPopulation.clear();
    nextPopulation.clear();
}

void GeneticAlgorithm::displayLatestResults() {
    std::cout << "START POPULATION BEST COST: " << startingPopulationBestPathCost << std::endl;
    std::cout << "ALGORITHM RESULTS:" << std::endl;
    std::cout << "Best Path: ";
    for (const auto &item: bestPath) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
    std::cout << "Best Cost: " << bestCost << std::endl;
}

void
GeneticAlgorithm::mainFun(int **matrix, int matrixSize, int populationSize, double mutationFactor, double crossFactor,
                          long long int startQPC, int timeout, bool isRandomInit, CrossMethod method) {
    clearMemory();

    this->matrix = matrix;
    this->matrixSize = matrixSize;
    this->populationSize = populationSize;
    this->mutationFactor = mutationFactor;
    this->crossFactor = crossFactor;
    this->startQPC = startQPC;
    this->timeoutSeconds = timeout;
    this->isRandomPopulationInitialization = isRandomInit;
    this->crossoverMethod = method;

    currentPopulation.reserve(populationSize);
    nextPopulation.reserve(populationSize);

    matingPoolSize = ceil(populationSize * crossFactor);
    matingPool.reserve(matingPoolSize);

    tournamentParticipants = ceil(sqrt(populationSize) / 3);
    if (tournamentParticipants < 3) {
        tournamentParticipants = 3;
    }

    eliteFactor = 1.0 - crossFactor;
    eliteCount = ceil(populationSize * eliteFactor);

    mutationPoolSize = ceil(matingPoolSize * mutationFactor);

    solveTSP();
}

void GeneticAlgorithm::solveTSP() {
    std::chrono::time_point breakAlgoTimePoint = std::chrono::system_clock::now() +
                                                 std::chrono::duration_cast<std::chrono::seconds>(
                                                         std::chrono::duration<int>(timeoutSeconds)
                                                 );
    std::random_device rdev;
    std::mt19937 twisterEngine(rdev());
    std::uniform_int_distribution<> distInt(0, populationSize - 1);

    // Inicjalizacja populacji poczatkowej
    if (isRandomPopulationInitialization) {
        initializePopulationWithRandomPaths();
    } else {
        initializePopulation();
    }

    for (int iteration = 0; true; ++iteration) {
        std::shuffle(currentPopulation.begin(), currentPopulation.end(), rdev);

        // Przerwanie algorytmu jesli osiagnieto kryterium stopu
        if ((breakAlgoTimePoint - std::chrono::system_clock::now()).count() < 0) {
            break;
        }

        // 1. Ocena przystosowania oraz tworzenie elity
        for (int j = 0; j < populationSize; ++j) {

            currentPopulation[j].setPathCost(matrix);
            // Zbieranie danych o najlepszym wyniku
            if (currentPopulation[j].pathCost < bestCost) {
                bestPath = currentPopulation[j].path;
                bestCost = currentPopulation[j].pathCost;
                bestCostFoundQPC = Timer::read_QPC();
            }

            if (currentElite.size() < eliteCount || currentPopulation[j].pathCost <= currentElite.back().pathCost) {
                currentElite.push_front(currentPopulation[j]);
                if (currentElite.size() > eliteCount) {
                    currentElite.pop_back();
                }
                currentElite.sort([](const GASubject &a, const GASubject &b) {
                    return a.pathCost < b.pathCost;
                });
            }

        }

        // 2. Selekcja
        while (matingPool.size() != matingPoolSize) {
            auto winners = tournamentSelection(distInt, twisterEngine);
            matingPool.push_back(winners.first);
            matingPool.push_back(winners.second);
        }

        // 3. Krzyżowanie oraz mutacja
        for (int j = 0; j < matingPoolSize - 1; j = j + 2) {
            auto offspring = crossSubjects(matingPool[j], matingPool[j + 1]);
            if (j <= mutationPoolSize * 2) {
                mutate(offspring.first);
            }
            nextPopulation.push_back(offspring.first);
            nextPopulation.push_back(offspring.second);
        }

        // 4. Uzupelnianie populacji elita -> sukcesja
        auto it = currentElite.begin();
        while (nextPopulation.size() != populationSize && it != currentElite.end()) {
            nextPopulation.push_back(*it);
            ++it;
        }

        // 5. Zastepowanie obecnej populacji nowa populacja
        currentPopulation.swap(nextPopulation);
        nextPopulation.clear();
        matingPool.clear();
        currentElite.clear();
    }

}

void GeneticAlgorithm::initializePopulation() {
    // Wyznaczanie rozwiazania przy pomocy metody zachlannej
    auto greedySolutions = greedyAlgorithm->getBestGreedyAlgorithmResult(matrix, matrixSize);
    startingPopulationBestPathCost = greedyAlgorithm->bestCost;

    int it = 0;
    while (it < matrixSize && currentPopulation.size() != populationSize) {
        currentPopulation.emplace_back(greedySolutions[it].first, greedySolutions[it].second);
        if (greedySolutions[it].second < bestCost) {
            bestPath = greedySolutions[it].first;
            bestCost = greedySolutions[it].second;
            bestCostFoundQPC = Timer::read_QPC();
        }
        ++it;
    }

    std::random_device rdev;
    std::mt19937 gen(rdev());

    while (currentPopulation.size() != populationSize) {
        std::vector<int> path(greedyAlgorithm->bestPath);
        path.pop_back();

        int v1 = RandomDataGenerator::generateRandomIntInRange(0, matrixSize - 1);
        int v2;
        do {
            v2 = RandomDataGenerator::generateRandomIntInRange(0, matrixSize - 1);
        } while (v2 == v1);

        int randomPerturbationOption = RandomDataGenerator::generateRandomIntInRange(0, 2);

        if (randomPerturbationOption == 0) {
            std::swap(path[v1], path[v2]);

        } else if (randomPerturbationOption == 1) {
            std::reverse(path.begin() + v1, path.begin() + v2);

        } else {
            v1 = RandomDataGenerator::generateRandomIntInRange(0, matrixSize - 1);
            do {
                v2 = RandomDataGenerator::generateRandomIntInRange(0, matrixSize - 1);
            } while (v2 == v1 || abs((v2 - v1)) > (matrixSize - 1 / 2));
            if (v1 > v2) {
                std::swap(v1, v2);
            }
            std::shuffle(path.begin() + v1, path.begin() + v2, gen);
        }

        path.push_back(path[0]);
        auto subject = GASubject(path);
        subject.setPathCost(matrix);

        if (subject.pathCost < bestCost) {
            bestPath = subject.path;
            bestCost = subject.pathCost;
            bestCostFoundQPC = Timer::read_QPC();
        }

        currentPopulation.push_back(subject);
    }
}

void GeneticAlgorithm::initializePopulationWithRandomPaths() {
    std::random_device rdev;
    std::mt19937 gen(rdev());

    while (currentPopulation.size() != populationSize) {
        std::vector<int> path(matrixSize);
        std::iota(std::begin(path), std::end(path), 0);
        std::shuffle(path.begin(), path.end(), gen);
        path.push_back(path[0]);

        auto subject = GASubject(path);
        subject.setPathCost(matrix);

        if (subject.pathCost < bestCost) {
            bestPath = subject.path;
            bestCost = subject.pathCost;
            bestCostFoundQPC = Timer::read_QPC();
        }

        currentPopulation.push_back(subject);
    }
}

std::pair<GASubject, GASubject>
GeneticAlgorithm::tournamentSelection(std::uniform_int_distribution<> &distInt, std::mt19937 &device) {
    int randomIndex = distInt(device);
    int secondRandomIndex;
    do {
        secondRandomIndex = distInt(device);
    } while (inRange(randomIndex, randomIndex + tournamentParticipants, secondRandomIndex) ||
             inRange(randomIndex, randomIndex + tournamentParticipants, secondRandomIndex + tournamentParticipants));

    GASubject a = currentPopulation[randomIndex++ % (populationSize - 1)];
    int limit = randomIndex + tournamentParticipants;
    for (; randomIndex < limit; ++randomIndex) {
        int i = randomIndex % (populationSize - 1);
        if (currentPopulation[i].pathCost <= a.pathCost) {
            a = currentPopulation[i];
        }
    }

    GASubject b = currentPopulation[secondRandomIndex++ % (populationSize - 1)];
    int limit2 = secondRandomIndex + tournamentParticipants;
    for (; secondRandomIndex < limit2; ++secondRandomIndex) {
        int i = secondRandomIndex % (populationSize - 1);
        if (currentPopulation[i].pathCost <= b.pathCost) {
            b = currentPopulation[i];
        }
    }

    if (a.pathCost > b.pathCost) {
        return std::make_pair(b, a);
    }
    return std::make_pair(a, b);
}

std::pair<GASubject, GASubject> GeneticAlgorithm::crossSubjects(GASubject &first, GASubject &second) const {
    std::pair<std::vector<int>, std::vector<int>> descendants;
    if (crossoverMethod == CrossMethod::OX) {
        descendants = CrossoverMethods::orderCrossover(first.path, second.path, matrixSize);
    } else {
        descendants = CrossoverMethods::edgeCrossover(first.path, second.path, matrixSize);
    }

    auto subject1 = GASubject(descendants.first);
    auto subject2 = GASubject(descendants.second);
    return std::make_pair(subject1, subject2);
}

void GeneticAlgorithm::mutate(GASubject &subject) const {
    subject.path.pop_back();
    int v1 = RandomDataGenerator::generateRandomIntInRange(0, matrixSize - 1);
    int v2;
    do {
        v2 = RandomDataGenerator::generateRandomIntInRange(0, matrixSize - 1);
    } while (v2 == v1);

    std::swap(subject.path[v1], subject.path[v2]);
    subject.path.push_back(subject.path[0]);
}
