#pragma once

#include <QPlainTextEdit>

class IterationLogger {
public:
    explicit IterationLogger(QPlainTextEdit *output);
    void clear();
    void log(const QString &line);

private:
    QPlainTextEdit *output_ = nullptr;
};
