#ifndef PEA_PWR_3_RANDOMDATAGENERATOR_H
#define PEA_PWR_3_RANDOMDATAGENERATOR_H

#include <iostream>
#include <random>

class RandomDataGenerator {
public:
    static double generateRandomDouble(double from, double to);

    static int generateRandomIntInRange(int from, int to);
};


#endif //PEA_PWR_3_RANDOMDATAGENERATOR_H
