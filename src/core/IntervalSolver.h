#pragma once

#include <string>
#include <vector>

#include "core/Parser.h"

enum class IntervalMode {
    RealInput = 0,
    IntervalInput = 1
};

struct IntervalResult {
    bool ok = false;
    std::string left;
    std::string right;
    std::string width;
    std::vector<std::string> log;
    std::string error;
};

class IntervalSolver {
public:
    static IntervalResult solve(const ParsedInput &input, IntervalMode mode, int maxIter);
};
