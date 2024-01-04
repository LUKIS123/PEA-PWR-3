#include <iostream>
#include <random>
#include "./Flow/AppController.h"

#include "./Utils/CrossoverMethods.h"

int main() {
    std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> v2{9, 3, 7, 8, 2, 6, 5, 1, 4};
    for (auto &item: v1) {
        --item;
    }
    for (auto &item: v2) {
        --item;
    }

    CrossoverMethods::EdgeCrossover(v1, v2, 9);

    srand(time(NULL));
    auto *controller = new AppController();
    controller->mainIndex();
    delete controller;
    return 0;
}
