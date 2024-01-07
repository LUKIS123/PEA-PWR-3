#include "GeneticAlgorithm.h"

GeneticAlgorithm::~GeneticAlgorithm() = default;

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
    std::cout << "GREEDY: " << greedyAlgorithmCost << std::endl;
    std::cout << "ALGORITHM RESULTS:" << std::endl;
    std::cout << "Best Path: ";
    for (const auto &item: bestPath) {
        std::cout << item << ", ";
    }
    std::cout << std::endl;
    std::cout << "Best Cost: " << bestCost << std::endl;
}

void
GeneticAlgorithm::mainFun(int **matrix, int matrixSize, int populationSize, double mutationFactor,
                          double crossFactor, long long int startQPC, int timeout) {
    clearMemory();

    this->matrix = matrix;
    this->matrixSize = matrixSize;
    this->populationSize = populationSize;
    this->mutationFactor = mutationFactor;
    this->crossFactor = crossFactor;
    this->startQPC = startQPC;
    this->timeoutSeconds = timeout;

    currentPopulation.reserve(populationSize);
    nextPopulation.reserve(populationSize);

    tournamentParticipants = floor(sqrt(populationSize));
    eliteCount = populationSize * eliteFactor;

    solveTSP();
}

void GeneticAlgorithm::solveTSP() {
    std::chrono::time_point breakAlgoTimePoint = std::chrono::system_clock::now() +
                                                 std::chrono::duration_cast<std::chrono::seconds>(
                                                         std::chrono::duration<int>(timeoutSeconds)
                                                 );

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::random_device rd;
    std::mt19937 gen(rd());

    // Inicjalizacja populacji poczatkowej
    initializePopulation();

    for (int iteration = 0; true; ++iteration) {

        // Przerwanie algorytmu jesli osiagnieto kryterium stopu
        if ((breakAlgoTimePoint - std::chrono::system_clock::now()).count() < 0) {
            break;
        }

//        currentElite.clear();
//        for (int j = 0; j < matrixSize; ++j) {
//
//            if (currentElite.size() < eliteCount || currentPopulation[j].pathCost <= currentElite.back().pathCost) {
//                currentElite.push_front(currentPopulation[j]);
//                if (currentElite.size() > eliteCount) {
//                    currentElite.pop_back();
//                }
//                currentElite.sort([](const GASubject &a, const GASubject &b) {
//                    return a.pathCost < b.pathCost;
//                });
//            }
//
//            if (currentPopulation[j].pathCost < bestCost) {
//                bestPath = currentPopulation[j].path;
//                bestCost = currentPopulation[j].pathCost;
//                bestCostFoundQPC = Timer::read_QPC();
//            }
//
//        }
//
//        // Zapewnienie przetrwania najlepszym osobnikom z poprzedniej populacji
//        std::copy(std::begin(currentElite), std::end(currentElite), std::back_inserter(nextPopulation));

        while (nextPopulation.size() != populationSize) {
            auto winners = tournamentSelection();

            if (dist(gen) < crossFactor) {
                // Krzyzowanie
                auto offspring = crossSubjects(winners.first, winners.second);
                // Mutacja
                if (dist(gen) < mutationFactor) {
                    mutate(offspring);
                }
                offspring.setPathCost(matrix);


                if (offspring.pathCost < bestCost) {
                    bestPath = offspring.path;
                    bestCost = offspring.pathCost;
                    bestCostFoundQPC = Timer::read_QPC();
                }

                nextPopulation.push_back(offspring);

            } else {
                nextPopulation.push_back(winners.first);
            }

        }

        // Zastepowanie obecnej populacji nowa populacja
        currentPopulation.swap(nextPopulation);
        nextPopulation.clear();
    }

}

void GeneticAlgorithm::initializePopulation() {
    // Wyznaczanie rozwiazania przy pomocy metody zachlannej
    auto pathCostPair = GreedyAlgorithm::getBestGreedyAlgorithmResult(matrix, matrixSize);
    greedyAlgorithmCost = pathCostPair.second;

    auto greedy = GASubject(pathCostPair.first);
    currentPopulation.push_back(greedy);
    greedy.setPathCost(matrix);

    std::random_device rdev;
    std::mt19937 gen(rdev());

    for (int i = 1; i < populationSize; ++i) {
        std::vector<int> path(pathCostPair.first);
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
            } while (v2 == v1 || abs((v2 - v1)) > (matrixSize / 2));
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

std::pair<GASubject, GASubject> GeneticAlgorithm::tournamentSelection() {
    int randomIndex = RandomDataGenerator::generateRandomIntInRange(0, matrixSize - 1);

    GASubject a = currentPopulation[randomIndex++ % (matrixSize - 1)];
    int limit = randomIndex + tournamentParticipants;
    for (; randomIndex < limit; ++randomIndex) {
        int i = randomIndex % (matrixSize - 1);
        if (currentPopulation[i].pathCost <= a.pathCost) {
            a = currentPopulation[i];
        }
    }

    GASubject b = currentPopulation[randomIndex++ % (matrixSize - 1)];
    int limit2 = limit + tournamentParticipants;
    for (; randomIndex < limit2; ++randomIndex) {
        int i = randomIndex % (matrixSize - 1);
        if (currentPopulation[i].pathCost <= b.pathCost) {
            b = currentPopulation[i];
        }
    }

    if (a.pathCost > b.pathCost) {
        return std::make_pair(b, a);
    }
    return std::make_pair(a, b);
}

GASubject GeneticAlgorithm::crossSubjects(GASubject &first, GASubject &second) const {
    auto descendant = CrossoverMethods::orderCrossover(first.path, second.path, matrixSize);
    auto subject = GASubject(descendant.first);
    return subject;
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
