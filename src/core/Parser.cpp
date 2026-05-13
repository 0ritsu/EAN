#include "Parser.h"

#include <QString>

namespace {
QString stripWhitespace(const QString &text) {
    QString out;
    out.reserve(text.size());
    for (QChar ch : text) {
        if (!ch.isSpace()) {
            out.append(ch);
        }
    }
    return out;
}

bool parseElement(const QString &s, int &i, QString &token, QString &error) {
    const int n = s.size();
    if (i >= n) {
        error = "Nieoczekiwany koniec danych.";
        return false;
    }

    if (s[i] == '[') {
        int start = i;
        int depth = 0;
        while (i < n) {
            if (s[i] == '[') {
                depth++;
            } else if (s[i] == ']') {
                depth--;
                if (depth == 0) {
                    i++;
                    token = s.mid(start, i - start);
                    return true;
                }
            }
            i++;
        }
        error = "Nie domknięto przedziału.";
        return false;
    }

    int start = i;
    while (i < n && s[i] != ',' && s[i] != ']') {
        i++;
    }
    token = s.mid(start, i - start);
    if (token.isEmpty()) {
        error = "Pusty element w danych.";
        return false;
    }
    return true;
}

bool parseRow(const QString &s, int &i, std::vector<QString> &row, QString &error) {
    const int n = s.size();
    if (i >= n || s[i] != '[') {
        error = "Oczekiwano '[' dla wiersza macierzy.";
        return false;
    }
    i++;
    while (i < n) {
        if (s[i] == ']') {
            i++;
            return true;
        }

        QString token;
        if (!parseElement(s, i, token, error)) {
            return false;
        }
        row.push_back(token);

        if (i < n && s[i] == ',') {
            i++;
        } else if (i < n && s[i] == ']') {
            continue;
        } else if (i >= n) {
            error = "Nieoczekiwany koniec wiersza.";
            return false;
        } else {
            error = "Niepoprawny separator w wierszu.";
            return false;
        }
    }

    error = "Nie domknięto wiersza macierzy.";
    return false;
}

bool parseMatrix(const QString &s, std::vector<std::vector<QString>> &A, QString &error) {
    int i = 0;
    const int n = s.size();
    if (n == 0 || s[i] != '[') {
        error = "Macierz musi zaczynać się od '['.";
        return false;
    }
    i++;
    while (i < n) {
        if (s[i] == ']') {
            i++;
            break;
        }
        std::vector<QString> row;
        if (!parseRow(s, i, row, error)) {
            return false;
        }
        A.push_back(row);

        if (i < n && s[i] == ',') {
            i++;
            continue;
        }
        if (i < n && s[i] == ']') {
            i++;
            break;
        }
    }

    if (A.empty()) {
        error = "Macierz jest pusta.";
        return false;
    }

    const size_t cols = A.front().size();
    for (const auto &row : A) {
        if (row.size() != cols) {
            error = "Macierz ma różne długości wierszy.";
            return false;
        }
    }
    return true;
}

bool parseVector(const QString &s, std::vector<QString> &b, QString &error) {
    int i = 0;
    const int n = s.size();
    if (n == 0 || s[i] != '[') {
        error = "Wektor musi zaczynać się od '['.";
        return false;
    }
    i++;
    while (i < n) {
        if (s[i] == ']') {
            i++;
            break;
        }
        QString token;
        if (!parseElement(s, i, token, error)) {
            return false;
        }
        b.push_back(token);

        if (i < n && s[i] == ',') {
            i++;
            continue;
        }
        if (i < n && s[i] == ']') {
            i++;
            break;
        }
    }

    if (b.empty()) {
        error = "Wektor jest pusty.";
        return false;
    }
    return true;
}
}

ParsedInput Parser::parse(const QString &text) {
    ParsedInput result;
    const QString clean = stripWhitespace(text);

    const int aPos = clean.indexOf("A=");
    const int bPos = clean.indexOf("b=");
    if (aPos < 0 || bPos < 0 || bPos <= aPos) {
        result.ok = false;
        result.error = "Wejście musi zawierać A=... oraz b=...";
        return result;
    }

    const QString aPart = clean.mid(aPos + 2, bPos - (aPos + 2));
    const QString bPart = clean.mid(bPos + 2);

    QString error;
    if (!parseMatrix(aPart, result.A, error)) {
        result.ok = false;
        result.error = "Błąd macierzy A: " + error;
        return result;
    }

    if (!parseVector(bPart, result.b, error)) {
        result.ok = false;
        result.error = "Błąd wektora b: " + error;
        return result;
    }

    if (result.A.size() != result.b.size()) {
        result.ok = false;
        result.error = "Rozmiar b nie pasuje do macierzy A.";
        return result;
    }

    result.ok = true;
    return result;
}

bool Parser::toDoubleMatrix(const ParsedInput &input,
                            std::vector<std::vector<double>> &A,
                            std::vector<double> &b,
                            QString &error) {
    A.clear();
    b.clear();

    try {
        for (const auto &row : input.A) {
            std::vector<double> outRow;
            outRow.reserve(row.size());
            for (const auto &cell : row) {
                if (cell.startsWith('[')) {
                    error = "W trybie normalnym nie wolno używać przedziałów.";
                    return false;
                }
                bool ok = false;
                double v = cell.toDouble(&ok);
                if (!ok) {
                    error = "Niepoprawna liczba w macierzy A: " + cell;
                    return false;
                }
                outRow.push_back(v);
            }
            A.push_back(outRow);
        }

        for (const auto &cell : input.b) {
            if (cell.startsWith('[')) {
                error = "W trybie normalnym nie wolno używać przedziałów.";
                return false;
            }
            bool ok = false;
            double v = cell.toDouble(&ok);
            if (!ok) {
                error = "Niepoprawna liczba w wektorze b: " + cell;
                return false;
            }
            b.push_back(v);
        }
    } catch (...) {
        error = "Błąd konwersji danych do double.";
        return false;
    }

    return true;
}
