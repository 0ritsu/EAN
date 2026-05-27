#pragma once

#include <QPlainTextEdit>

class ComputationLogger {
public:
    explicit ComputationLogger(QPlainTextEdit *output);
    void clear();
    void log(const QString &line);

private:
    QPlainTextEdit *output_ = nullptr;
};
