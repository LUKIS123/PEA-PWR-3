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

    tournamentParticipants = ceil(sqrt(populationSize) / 2);
    if (tournamentParticipants < 2) {
        tournamentParticipants = 2;
    }

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
    std::uniform_real_distribution<double> distReal(0.0, 1.0);


    // Inicjalizacja populacji poczatkowej
    initializePopulation();
    std::shuffle(currentPopulation.begin(), currentPopulation.end(), rdev);

    for (int iteration = 0; true; ++iteration) {

        // Przerwanie algorytmu jesli osiagnieto kryterium stopu
        if ((breakAlgoTimePoint - std::chrono::system_clock::now()).count() < 0) {
            break;
        }

        // Przetrwanie najlepszego potomka
        GASubject best = GASubject(bestPath);
        best.setPathCost(matrix);
        nextPopulation.push_back(best);

        while (nextPopulation.size() != populationSize) {
            auto winners = tournamentSelection(distInt, rdev);

            if (distReal(twisterEngine) < crossFactor) {

                // Krzyzowanie -> zwracanych jest 2 potomkow
                auto crossResult = crossSubjects(winners.first, winners.second);

                // Mutacja
                if (distReal(twisterEngine) < mutationFactor) {
                    mutate(crossResult.first);
                }
                insertOffspring(crossResult.first);

                if (nextPopulation.size() != populationSize) {
                    // Mutacja
                    if (distReal(twisterEngine) < mutationFactor) {
                        mutate(crossResult.second);
                    }
                    insertOffspring(crossResult.second);
                }

            } else {

                if (winners.first.pathCost > winners.second.pathCost) {
                    nextPopulation.push_back(winners.first);
                } else {
                    nextPopulation.push_back(winners.second);
                }

            }

        }

        // Zastepowanie obecnej populacji nowa populacja
        currentPopulation.swap(nextPopulation);
        nextPopulation.clear();
    }

}

void GeneticAlgorithm::initializePopulation() {
    // Wyznaczanie rozwiazania przy pomocy metody zachlannej
    auto greedySolutions = greedyAlgorithm->getBestGreedyAlgorithmResult(matrix, matrixSize);
    greedyAlgorithmCost = greedyAlgorithm->bestCost;

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

std::pair<GASubject, GASubject>
GeneticAlgorithm::tournamentSelection(std::uniform_int_distribution<> &distInt, std::random_device &device) {
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
    auto descendant = CrossoverMethods::orderCrossover(first.path, second.path, matrixSize);
    auto subject1 = GASubject(descendant.first);
    auto subject2 = GASubject(descendant.second);
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

void GeneticAlgorithm::insertOffspring(GASubject &subject) {
    subject.setPathCost(matrix);
    if (subject.pathCost < bestCost) {
        bestPath = subject.path;
        bestCost = subject.pathCost;
        bestCostFoundQPC = Timer::read_QPC();
    }
    nextPopulation.push_back(subject);
}

bool GeneticAlgorithm::inRange(unsigned int low, unsigned int high, unsigned int x) {
    return ((x - low) <= (high - low));
}
