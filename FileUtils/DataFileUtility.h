#ifndef PEA_PWR_3_DATAFILEUTILITY_H
#define PEA_PWR_3_DATAFILEUTILITY_H

#include "iostream"
#include "vector"
#include "../MeasureTime/Timer.h"

class DataFileUtility {
public:
    static bool saveResultPath(const std::string &FileName, const std::vector<int> &path);

    static std::vector<int> readPathFromFile(const std::string &FileName);

    static bool
    saveAutomaticSATestResults(const std::string &FileName, const std::vector<double> &us,
                               const std::vector<double> &ms,
                               const std::vector<double> &s, const std::vector<int> &greedyCosts,
                               const std::vector<int> &solCosts,
                               const std::vector<double> &endTemp, const std::vector<double> &exps, double factor,
                               const std::string &headline);

    static bool
    saveAutomaticGATestResults(const std::string &FileName, const std::vector<double> &us,
                               const std::vector<double> &ms,
                               const std::vector<double> &s, const std::vector<int> &beginCosts,
                               const std::vector<int> &solCosts, const std::string &headline, const int size);

    static bool
    saveTimestamps(const std::string &FileName, const std::vector<std::vector<double>> &timestamps);

    static bool
    saveProgressionPoints(const std::string &FileName, const std::vector<std::vector<int>> &progressionPoints);
};


#endif //PEA_PWR_3_DATAFILEUTILITY_H
