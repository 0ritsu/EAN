#include "IntervalSolver.h"

#include <sstream>
#include <iomanip>

#include "interval/Interval.h"

using interval_arithmetic::Interval;
using interval_arithmetic::LeftRead;
using interval_arithmetic::RightRead;
using interval_arithmetic::IAdd;
using interval_arithmetic::ISub;
using interval_arithmetic::IMul;
using interval_arithmetic::IDiv;
using interval_arithmetic::ISqrt;
using interval_arithmetic::PINT_MODE;

namespace {
std::string formatDouble(long double v) {
    std::ostringstream oss;
    oss << std::setprecision(17) << v;
    return oss.str();
}

bool parseIntervalToken(const QString &token, Interval<long double> &out, std::string &error) {
    if (token.startsWith('[')) {
        if (!token.endsWith(']')) {
            error = "Nie domknięto przedziału.";
            return false;
        }
        QString inner = token.mid(1, token.size() - 2);
        int comma = inner.indexOf(',');
        if (comma <= 0) {
            error = "Przedział musi mieć format [a,b].";
            return false;
        }
        QString left = inner.left(comma);
        QString right = inner.mid(comma + 1);
        out.a = LeftRead<long double>(left.toStdString());
        out.b = RightRead<long double>(right.toStdString());
        return true;
    }

    out.a = LeftRead<long double>(token.toStdString());
    out.b = RightRead<long double>(token.toStdString());
    return true;
}
}

IntervalResult IntervalSolver::solve(const ParsedInput &input, IntervalMode mode, int maxIter) {
    IntervalResult result;
    const size_t n = input.A.size();
    if (n == 0 || input.b.size() != n) {
        result.ok = false;
        result.error = "Niepoprawne rozmiary A lub b.";
        return result;
    }

    Interval<long double>::Initialize();
    Interval<long double>::SetMode(PINT_MODE);

    std::vector<std::vector<Interval<long double>>> A(n, std::vector<Interval<long double>>(n));
    std::vector<Interval<long double>> b(n);

    for (size_t i = 0; i < n; ++i) {
        if (input.A[i].size() != n) {
            result.ok = false;
            result.error = "Macierz A musi być kwadratowa.";
            return result;
        }
        for (size_t j = 0; j < n; ++j) {
            Interval<long double> cell;
            if (!parseIntervalToken(input.A[i][j], cell, result.error)) {
                result.ok = false;
                return result;
            }
            A[i][j] = cell;
        }
    }

    for (size_t i = 0; i < n; ++i) {
        Interval<long double> cell;
        if (!parseIntervalToken(input.b[i], cell, result.error)) {
            result.ok = false;
            return result;
        }
        b[i] = cell;
    }

    std::vector<std::vector<Interval<long double>>> L(n, std::vector<Interval<long double>>(n, Interval<long double>(0, 0)));
    int steps = 0;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j <= i; ++j) {
            if (++steps > maxIter) {
                result.ok = false;
                result.error = "Przekroczono maksymalną liczbę iteracji.";
                return result;
            }

            Interval<long double> sum = A[i][j];
            for (size_t k = 0; k < j; ++k) {
                sum = ISub(sum, IMul(L[i][k], L[j][k]));
            }

            if (i == j) {
                int st = 0;
                Interval<long double> root = ISqrt(sum, st);
                if (st != 0) {
                    result.ok = false;
                    result.error = "Nie można wyznaczyć pierwiastka (ujemny przedział).";
                    return result;
                }
                L[i][j] = root;
            } else {
                L[i][j] = IDiv(sum, L[j][j]);
            }

            std::string left, right;
            L[i][j].IEndsToStrings(left, right);
            result.log.push_back("L[" + std::to_string(i) + "," + std::to_string(j) + "] = [" + left + "," + right + "]");
        }
    }

    std::vector<Interval<long double>> y(n, Interval<long double>(0, 0));
    for (size_t i = 0; i < n; ++i) {
        if (++steps > maxIter) {
            result.ok = false;
            result.error = "Przekroczono maksymalną liczbę iteracji.";
            return result;
        }
        Interval<long double> sum = b[i];
        for (size_t k = 0; k < i; ++k) {
            sum = ISub(sum, IMul(L[i][k], y[k]));
        }
        y[i] = IDiv(sum, L[i][i]);
        std::string left, right;
        y[i].IEndsToStrings(left, right);
        result.log.push_back("y[" + std::to_string(i) + "] = [" + left + "," + right + "]");
    }

    std::vector<Interval<long double>> x(n, Interval<long double>(0, 0));
    for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
        if (++steps > maxIter) {
            result.ok = false;
            result.error = "Przekroczono maksymalną liczbę iteracji.";
            return result;
        }
        Interval<long double> sum = y[i];
        for (size_t k = i + 1; k < n; ++k) {
            sum = ISub(sum, IMul(L[k][i], x[k]));
        }
        x[i] = IDiv(sum, L[i][i]);
        std::string left, right;
        x[i].IEndsToStrings(left, right);
        result.log.push_back("x[" + std::to_string(i) + "] = [" + left + "," + right + "]");
    }

    std::ostringstream leftOut;
    std::ostringstream rightOut;
    std::ostringstream widthOut;
    leftOut << "[";
    rightOut << "[";
    widthOut << "[";
    for (size_t i = 0; i < n; ++i) {
        std::string left, right;
        x[i].IEndsToStrings(left, right);
        long double width = x[i].GetWidth();

        if (i > 0) {
            leftOut << ", ";
            rightOut << ", ";
            widthOut << ", ";
        }
        leftOut << left;
        rightOut << right;
        widthOut << formatDouble(width);
    }
    leftOut << "]";
    rightOut << "]";
    widthOut << "]";

    result.ok = true;
    result.left = leftOut.str();
    result.right = rightOut.str();
    result.width = widthOut.str();
    return result;
}
