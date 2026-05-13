#include "IterationLogger.h"

IterationLogger::IterationLogger(QPlainTextEdit *output) : output_(output) {}

void IterationLogger::clear() {
    if (output_) {
        output_->clear();
    }
}

void IterationLogger::log(const QString &line) {
    if (output_) {
        output_->appendPlainText(line);
    }
}
