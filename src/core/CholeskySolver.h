#pragma once

#include <vector>

struct CholeskyResult {
    bool ok = false;
    std::vector<double> x;
    std::string error;
};

class CholeskySolver {
public:
    // TODO: implementacja Choleskiego dla SPD
    static CholeskyResult solve(const std::vector<std::vector<double>> &A,
                                const std::vector<double> &b,
                                int maxIter);
};
