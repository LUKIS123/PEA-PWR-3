#include "RandomDataGenerator.h"


double RandomDataGenerator::generateRandomDouble(double from, double to) {
    std::uniform_real_distribution<double> dist(from, to);
    std::random_device rd;
    std::mt19937 gen(rd());
    return dist(gen);
}

int RandomDataGenerator::generateRandomIntInRange(int from, int to) {
    std::random_device rdev;
    std::mt19937 gen(rdev());
    std::uniform_int_distribution<> dist(from, to);
    return dist(gen);
}
