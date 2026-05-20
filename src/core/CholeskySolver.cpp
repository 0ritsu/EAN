#include "CholeskySolver.h"

#include <cmath>
#include <sstream>
#include <iomanip>

namespace {
std::string formatDouble(double v) {
    std::ostringstream oss;
    oss << std::setprecision(17) << v;
    return oss.str();
}
}

CholeskyResult CholeskySolver::solve(const std::vector<std::vector<double>> &A,
                                     const std::vector<double> &b) {
    CholeskyResult result;
    const size_t n = A.size();
    if (n == 0 || b.size() != n) {
        result.ok = false;
        result.error = "Niepoprawne rozmiary A lub b.";
        return result;
    }
    for (const auto &row : A) {
        if (row.size() != n) {
            result.ok = false;
            result.error = "Macierz A musi być kwadratowa.";
            return result;
        }
    }

    const double eps = 1e-12;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (std::abs(A[i][j] - A[j][i]) > eps) {
                result.ok = false;
                result.error = "Macierz A nie jest symetryczna.";
                return result;
            }
        }
    }

    std::vector<std::vector<double>> L(n, std::vector<double>(n, 0.0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            double sum = A[i][j];
            for (size_t k = 0; k < j; ++k) {
                sum -= L[i][k] * L[j][k];
            }

            if (i == j) {
                if (sum <= 0.0) {
                    result.ok = false;
                    result.error = "Macierz nie jest dodatnio określona.";
                    return result;
                }
                L[i][j] = std::sqrt(sum);
            } else {
                L[i][j] = sum / L[j][j];
            }

            result.log.push_back("L[" + std::to_string(i) + "," + std::to_string(j) + "] = " + formatDouble(L[i][j]));
        }
    }

    std::vector<double> y(n, 0.0);
    for (size_t i = 0; i < n; ++i) {
        double sum = b[i];
        for (size_t k = 0; k < i; ++k) {
            sum -= L[i][k] * y[k];
        }
        y[i] = sum / L[i][i];
        result.log.push_back("y[" + std::to_string(i) + "] = " + formatDouble(y[i]));
    }

    std::vector<double> x(n, 0.0);
    for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
        double sum = y[i];
        for (size_t k = i + 1; k < n; ++k) {
            sum -= L[k][i] * x[k];
        }
        x[i] = sum / L[i][i];
        result.log.push_back("x[" + std::to_string(i) + "] = " + formatDouble(x[i]));
    }

    result.ok = true;
    result.x = x;
    return result;
}
