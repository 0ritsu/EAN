#pragma once

#include <QString>
#include <vector>

struct ParsedInput {
    bool ok = false;
    QString error;
    std::vector<std::vector<QString>> A;
    std::vector<QString> b;
};

class Parser {
public:
    static ParsedInput parse(const QString &text);
    static bool toDoubleMatrix(const ParsedInput &input,
                               std::vector<std::vector<double>> &A,
                               std::vector<double> &b,
                               QString &error);
};
