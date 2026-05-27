#include "ComputationLogger.h"

ComputationLogger::ComputationLogger(QPlainTextEdit *output) : output_(output) {}

void ComputationLogger::clear() {
    if (output_) {
        output_->clear();
    }
}

void ComputationLogger::log(const QString &line) {
    if (output_) {
        output_->appendPlainText(line);
    }
}
