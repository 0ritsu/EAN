#pragma once

#include <QString>

struct ParseResult {
    bool ok = false;
    QString error;
    QString raw;
};

class Parser {
public:
    static ParseResult parse(const QString &text);
};
