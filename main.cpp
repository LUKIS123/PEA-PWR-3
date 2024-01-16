#include <iostream>
#include <random>
#include "./Flow/AppController.h"

int main() {
    // permutation check
//    auto path = std::list<int>(result1.begin(), result1.end());
//    path.sort();
//
//    std::list<int> check(matrixSize);
//    std::iota(std::begin(check), std::end(check), 0);
//    check.push_back(result1.front());
//    check.sort();
//
//    if (check != path) {
//        int idx = 0;
//        auto itr = path.begin();
//        std::cout << "ERRROR" << std::endl;
//        for (const auto &item: check) {
//            if (item != *itr) {
//                std::cout << "ERRROR at" << idx << std::endl;
//            }
//            ++idx;
//            ++itr;
//        }
//    }


    srand(time(NULL));
    auto *controller = new AppController();
    controller->mainIndex();
    delete controller;
    return 0;
}
