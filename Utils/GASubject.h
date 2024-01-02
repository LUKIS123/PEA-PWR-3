#ifndef PEA_PWR_3_GASUBJECT_H
#define PEA_PWR_3_GASUBJECT_H

#include <iostream>
#include <vector>

class GASubject {
public:
    GASubject();

    ~GASubject() = default;

    std::vector<int> path;
    int pathCost = INT_MAX;
    double fitness = 0.0;

    explicit GASubject(const std::vector<int> &path);

    void setPathCost(int **matrix);

    void setFitness(int populationTotalCosts, int populationSize);

    bool operator<(const GASubject &other) const {
        return pathCost < other.pathCost;
    }

    bool operator>(const GASubject &other) const {
        return pathCost > other.pathCost;
    }
};


#endif //PEA_PWR_3_GASUBJECT_H
