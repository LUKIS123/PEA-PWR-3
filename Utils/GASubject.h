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

    explicit GASubject(const std::vector<int> &path);

    GASubject(const std::vector<int> &path, int pathCost);

    void setPathCost(int **matrix);

    bool operator<(const GASubject &other) const {
        return pathCost < other.pathCost;
    }

    bool operator>(const GASubject &other) const {
        return pathCost > other.pathCost;
    }
};


#endif //PEA_PWR_3_GASUBJECT_H
