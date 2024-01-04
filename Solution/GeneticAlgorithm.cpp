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
    std::cout << "ALGORITHM RESULTS:" << std::endl;
    std::cout << "Path: ";
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

        currentElite.clear();
        GASubject bestSubject;
        for (int j = 0; j < matrixSize; ++j) {
            currentPopulation[j].setFitness(currentPopulationTotalCost, populationSize);
            if (currentPopulation[j].fitness >= 1.0) {
                // Kazdy osobnik otrzymuje tyle kopii w nowej populacji, ile wynosi jego wspolczynnik fitness
                for (int k = 0; k < currentPopulation[j].fitness; ++k) {
                    nextPopulation.push_back(currentPopulation[j]);
                }
            }

            if (currentPopulation[j].pathCost <= bestSubject.pathCost) {
                bestSubject = currentPopulation[j];
            }
            if (currentPopulation[j].pathCost <= currentElite.back().pathCost || currentElite.size() < eliteCount) {
                currentElite.push_front(currentPopulation[j]);
                if (currentElite.size() > eliteCount) {
                    currentElite.pop_back();
                }
            }
            currentElite.sort([](const GASubject &a, const GASubject &b) { return a.pathCost < b.pathCost; });
        }
        // Zapewnienie przetrwania najlepszym osobnikom z poprzedniej populacji
        if (nextPopulation.size() < eliteCount) {
            std::copy(std::begin(currentElite), std::end(currentElite), std::back_inserter(nextPopulation));
        }
        bestPath = bestSubject.path;
        bestCost = bestSubject.pathCost;


        // todo: reszta wybierana turniejami
        /// wybieramy rodzicow poprzez turnieje ( rozmiar k uzaleznic od wielkosci populacji)
        /// nastepnie dwoch rodzicow z turnieju crossover
        /// nastepnie mutacja
        /// nastepnie dodawaj do nextPopulation i powtarzaj proces do ktorego nextPop.size == populationSize
        // UWAGA W CROSSS UWZGLEDNIC cross_factor! nie wszystkie beda sie crossowac
        // -> turniej jesli wpadnie cross to krzyzuje z drygum najlepszym , jesli nie to do nexta idzie najlepszy

        while (nextPopulation.size() != populationSize) {
            auto winners = tournamentSelection();
            if (dist(gen) < crossFactor) {
                auto offspring = crossSubjects(winners.first, winners.second);
                // Mutacja
                if (dist(gen) < mutationFactor) {
                    mutate(offspring);
                }
                offspring.setPathCost(matrix);
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

    int totalPopulationCost = 0;

    auto greedy = GASubject(pathCostPair.first);
    greedy.setPathCost(matrix);
    totalPopulationCost += greedy.pathCost;
    currentPopulation.push_back(greedy);

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

std::pair<GASubject, GASubject> GeneticAlgorithm::tournamentSelection() {
    int randomIndex = RandomDataGenerator::generateRandomIntInRange(0, matrixSize - 1);
    int limit = randomIndex + (tournamentParticipants * RandomDataGenerator::generateRandomDouble(0.75, 1.25));

    GASubject first = currentPopulation[randomIndex++ % (matrixSize - 1)];
    GASubject second = currentPopulation[randomIndex++ % (matrixSize - 1)];

    for (; randomIndex < limit; ++randomIndex) {
        int index = randomIndex % (matrixSize - 1);
        if (currentPopulation[index].pathCost <= first.pathCost) {
            first = currentPopulation[index];
        } else if (currentPopulation[index].pathCost <= second.pathCost) {
            second = currentPopulation[index];
        }
    }
    return std::make_pair(first, second);
}

GASubject GeneticAlgorithm::crossSubjects(GASubject &first, GASubject &second) {

}

void GeneticAlgorithm::mutate(GASubject &subject) {

}
