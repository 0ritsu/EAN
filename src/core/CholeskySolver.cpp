#include "CholeskySolver.h"

CholeskyResult CholeskySolver::solve(const std::vector<std::vector<double>> &A,
                                     const std::vector<double> &b,
                                     int maxIter) {
    (void)A;
    (void)b;
    (void)maxIter;
    CholeskyResult result;
    result.ok = false;
    result.error = "TODO: implementacja Choleskiego";
    return result;
}
