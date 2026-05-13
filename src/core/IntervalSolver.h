#pragma once

#include <string>

struct IntervalResult {
    bool ok = false;
    std::string left;
    std::string right;
    std::string width;
    std::string error;
};

class IntervalSolver {
public:
    // TODO: implementacja arytmetyki przedziałowej
    static IntervalResult solve(const std::string &rawInput, int maxIter, int mode);
};
