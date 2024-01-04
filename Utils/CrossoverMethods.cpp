#include "CrossoverMethods.h"


// Metoda implementujaca metode krzyzowania Order Crossover
std::pair<std::vector<int>, std::vector<int>>
CrossoverMethods::OrderCrossover(const std::vector<int> &first, const std::vector<int> &second, int matrixSize) {
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

// Metoda implementujaca metode krzyzowania Edge Crossover
std::vector<int>
CrossoverMethods::EdgeCrossover(const std::vector<int> &first, const std::vector<int> &second, int matrixSize) {
//    std::vector<int> values = first;
//    values.pop_back();
//    std::vector<int> parent2 = second;
//    parent2.pop_back();

    std::vector<std::list<std::pair<int, bool>>> edgeTab(matrixSize);

    edgeTab[first[0]].emplace_back(first[matrixSize - 1], false);
    edgeTab[first[0]].emplace_back(first[1], false);

    auto itFirstElemFirst = findElementOfEdgeList(edgeTab[second[0]], second[matrixSize - 1]);
    if (itFirstElemFirst == edgeTab[second[0]].end()) {
        edgeTab[second[0]].emplace_back(second[matrixSize - 1], false);
    } else {
        itFirstElemFirst->second = true;
    }

    auto itFirstElemSecond = findElementOfEdgeList(edgeTab[second[0]], second[1]);
    if (itFirstElemSecond == edgeTab[second[0]].end()) {
        edgeTab[second[0]].emplace_back(second[1], false);
    } else {
        itFirstElemSecond->second = true;
    }

    for (int i = 1; i < matrixSize - 1; ++i) {
        auto itFirst = findElementOfEdgeList(edgeTab[first[i]], first[i - 1]);
        if (itFirst == edgeTab[first[i]].end()) {
            edgeTab[first[i]].emplace_back(first[i - 1], false);
        } else {
            itFirst->second = true;
        }

        auto itSecond = findElementOfEdgeList(edgeTab[first[i]], first[i + 1]);
        if (itSecond == edgeTab[first[i]].end()) {
            edgeTab[first[i]].emplace_back(first[i + 1], false);
        } else {
            itSecond->second = true;
        }

        itFirst = findElementOfEdgeList(edgeTab[second[i]], second[i - 1]);
        if (itFirst == edgeTab[second[i]].end()) {
            edgeTab[second[i]].emplace_back(second[i - 1], false);
        } else {
            itFirst->second = true;
        }

        itSecond = findElementOfEdgeList(edgeTab[second[i]], second[i + 1]);
        if (itSecond == edgeTab[second[i]].end()) {
            edgeTab[second[i]].emplace_back(second[i + 1], false);
        } else {
            itSecond->second = true;
        }
    }

    auto itLastElemFirstFirst = findElementOfEdgeList(edgeTab[first[matrixSize - 1]], first[matrixSize - 2]);
    if (itLastElemFirstFirst == edgeTab[first[matrixSize - 1]].end()) {
        edgeTab[first[matrixSize - 1]].emplace_back(first[matrixSize - 2], false);
    } else {
        itLastElemFirstFirst->second = true;
    }

    auto itLastElemFirstSecond = findElementOfEdgeList(edgeTab[first[matrixSize - 1]], first[0]);
    if (itLastElemFirstSecond == edgeTab[first[matrixSize - 1]].end()) {
        edgeTab[first[matrixSize - 1]].emplace_back(first[0], false);
    } else {
        itLastElemFirstSecond->second = true;
    }

    auto itLastElemSecondFirst = findElementOfEdgeList(edgeTab[second[matrixSize - 1]], second[matrixSize - 2]);
    if (itLastElemSecondFirst == edgeTab[second[matrixSize - 1]].end()) {
        edgeTab[second[matrixSize - 1]].emplace_back(second[matrixSize - 2], false);
    } else {
        itLastElemSecondFirst->second = true;
    }

    auto itLastElemSecondSecond = findElementOfEdgeList(edgeTab[second[matrixSize - 1]], second[0]);
    if (itLastElemSecondSecond == edgeTab[second[matrixSize - 1]].end()) {
        edgeTab[second[matrixSize - 1]].emplace_back(second[0], false);
    } else {
        itLastElemSecondSecond->second = true;
    }

    std::list<int> offspring(matrixSize);

    std::vector<int> vertexes = first;
    vertexes.pop_back();
    std::random_device rdev;
    std::mt19937 gen(rdev());
    std::shuffle(std::begin(vertexes), std::end(vertexes), gen);

    std::list<int> remaining(vertexes.begin(), vertexes.end());
    int v = remaining.back();
    remaining.pop_back();

//    for (int i = 0; i < matrixSize; ++i) {
//        for (auto &list: edgeTab) {
//            list.remove_if([&v](std::pair<int, bool> &element) { return (element.first == v); });
//        }
//        auto itRepeated = std::find_if(edgeTab[v].begin(), edgeTab[v].end(),
//                                       [](const std::pair<int, bool> &element) {
//                                           return element.second;
//                                       });
//        if (itRepeated == edgeTab[v].end()) {
//
//            if (!edgeTab[v].empty()) {
//
//                std::pair<int, int> shortestEdgeListVertexCount(0, INT_MAX);
//                for (const auto &item: edgeTab[v]) {
//                    if (edgeTab[item.first].size() <= shortestEdgeListVertexCount.second) {
//                        shortestEdgeListVertexCount = std::make_pair(item.first, edgeTab[item.first].size());
//                    }
//                }
//                offspring.push_back(v);
//                v = shortestEdgeListVertexCount.first;
//                continue;
//
//            } else {
//
//                itRepeated = std::find_if(edgeTab[offspring.front()].begin(), edgeTab[offspring.front()].end(),
//                                          [](const std::pair<int, bool> &element) {
//                                              return element.second;
//                                          });
//                if (itRepeated == edgeTab[offspring.front()].end()) {
//                    std::pair<int, int> shortestEdgeListVertexCount(0, INT_MAX);
//                    for (const auto &item: edgeTab[offspring.front()]) {
//                        if (edgeTab[item.first].size() <= shortestEdgeListVertexCount.second) {
//                            shortestEdgeListVertexCount = std::make_pair(item.first, edgeTab[item.first].size());
//                        }
//                    }
//                    offspring.push_front(v);
//                    v = shortestEdgeListVertexCount.first;
//                    continue;
//
//                } else {
//                    // todo
//                    // zamiana kierunku????
//                    offspring.push_front(v);
//                    v = itRepeated->first;
//                    continue;
//                }
//
//                // pkt 5
//                offspring.push_back(random);
//
//            }
//
//        } else {
//            offspring.push_back(v);
//            v = itRepeated->first;
//            continue;
//        }
//    }
//////////////////////////////////////////////////////////////////////////////////////////////////////
    bool direction = true;
    while (offspring.size() != matrixSize) {
        //2. usuń z tablicy krawędzi wszystkie odniesienia do v
        for (auto &list: edgeTab) {
            list.remove_if([&v](std::pair<int, bool> &element) { return (element.first == v); });
        }

        //3. Zbadaj listę krawędzi dla v.
        auto result = tryAddBestNeighbor(v, direction, offspring, edgeTab);
        // punkt 6 -> dodano wierzcholek
        if (result.first) {
            removeRemaining(v, remaining);
            v = result.second;
            continue;
        }

        // 4. Jeżeli lista krawędzi v jest pusta spróbuj wykonać pkt 3. dla drugiego końcaciągu
        auto otherEndResult = tryAddBestNeighbor(offspring.front(), direction, offspring, edgeTab);
        if (otherEndResult.first) {
            removeRemaining(v, remaining);
            direction = false;
            v = otherEndResult.second;
            continue;
        }

        //5. Jeżeli pkt. 4 się nie udał
        addOffspring(offspring, v, direction);
        direction = true;
        v = remaining.back();
        remaining.pop_back();
    }

    std::vector<int> result{std::make_move_iterator(std::begin(offspring)),
                            std::make_move_iterator(std::end(offspring))};
    result.push_back(result[0]);
    return result;
}

// Funkcja pomocnicza sluzaca znalezieniu wierzcholka w liscie sasiedztwa
std::_List_iterator<std::pair<int, bool>>
CrossoverMethods::findElementOfEdgeList(std::list<std::pair<int, bool>> &edgeList, int value) {
    return std::find_if(edgeList.begin(), edgeList.end(),
                        [&value](const std::pair<int, bool> &element) {
                            return element.first == value;
                        });
}

// Funkcja pomocnicza dodajaca wierzcholek do ciagu wierzchokow nowego potomka
void CrossoverMethods::addOffspring(std::list<int> &offspring, int vertex, bool direction) {
    if (direction) {
        offspring.push_back(vertex);
    } else {
        offspring.push_front(vertex);
    }
}

// Punkt 3: Jeżeli to możliwe przejdź do wierzchołka połączonego krawędzią wspólną lub wierzchołka którego listakrawędzi jest najkrótsza
std::pair<bool, int> CrossoverMethods::tryAddBestNeighbor(int vertex, bool direction, std::list<int> &offspring,
                                                          std::vector<std::list<std::pair<int, bool>>> &edgeTab) {

    auto itRepeated = std::find_if(edgeTab[vertex].begin(), edgeTab[vertex].end(),
                                   [](const std::pair<int, bool> &element) {
                                       return element.second;
                                   });

    if (itRepeated == edgeTab[vertex].end()) {

        if (!edgeTab[vertex].empty()) {

            std::pair<int, int> shortestEdgeListVertexCount(-1, INT_MAX);
            for (const auto &item: edgeTab[vertex]) {
                if (edgeTab[item.first].size() <= shortestEdgeListVertexCount.second) {
                    shortestEdgeListVertexCount = std::make_pair(item.first, edgeTab[item.first].size());
                }
            }
            addOffspring(offspring, vertex, direction);
            return std::make_pair(true, shortestEdgeListVertexCount.first);
        }

    } else {
        addOffspring(offspring, vertex, direction);
        return std::make_pair(true, itRepeated->first);
    }

    return std::make_pair(false, -1);
}

void CrossoverMethods::removeRemaining(int vertex, std::list<int> &rem) {
    auto itr = find(rem.begin(), rem.end(), vertex);
    if (itr != rem.end()) {
        rem.erase(itr);
    }
}
