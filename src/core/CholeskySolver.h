#pragma once

#include <string>
#include <vector>

struct CholeskyResult {
    bool ok = false;
    std::vector<double> x;
    std::vector<std::string> log;
    std::string error;
};

class CholeskySolver {
public:
    static CholeskyResult solve(const std::vector<std::vector<double>> &A,
                                const std::vector<double> &b);
};
