#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "core/Parser.h"
#include "core/IterationLogger.h"
#include "core/CholeskySolver.h"
#include "core/IntervalSolver.h"

#include <sstream>
#include <iomanip>

namespace {
QString formatVector(const std::vector<double> &v) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i > 0) {
            oss << ", ";
        }
        oss << std::fixed << std::setprecision(17) << v[i];
    }
    oss << "]";
    return QString::fromStdString(oss.str());
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->computeButton, &QPushButton::clicked,
            this, &MainWindow::onComputeClicked);

    setStatus("Gotowe.");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setStatus(const QString &text, bool isError) {
    ui->statusLabel->setText(text);
    ui->statusLabel->setStyleSheet(isError ? "color: #c62828;" : "color: #2e7d32;");
}

void MainWindow::onComputeClicked() {
    IterationLogger logger(ui->logOutput);
    logger.clear();

    const QString rawInput = ui->inputEdit->toPlainText();
    if (rawInput.trimmed().isEmpty()) {
        setStatus("Wpisz dane wejściowe.", true);
        return;
    }

    ParsedInput parse = Parser::parse(rawInput);
    if (!parse.ok) {
        setStatus(parse.error, true);
        return;
    }

    const int maxIter = ui->maxIterSpin->value();

    if (ui->modeNormal->isChecked()) {
        std::vector<std::vector<double>> A;
        std::vector<double> b;
        QString error;
        if (!Parser::toDoubleMatrix(parse, A, b, error)) {
            setStatus(error, true);
            return;
        }

        CholeskyResult result = CholeskySolver::solve(A, b, maxIter);
        for (const auto &line : result.log) {
            logger.log(QString::fromStdString(line));
        }
        if (!result.ok) {
            setStatus(QString::fromStdString(result.error), true);
            return;
        }

        ui->leftOutput->setText(formatVector(result.x));
        ui->rightOutput->setText("-");
        ui->widthOutput->setText("-");
        setStatus("OK.");
        return;
    }

    IntervalMode mode = ui->modeIntervalInterval->isChecked() ? IntervalMode::IntervalInput
                                                             : IntervalMode::RealInput;
    IntervalResult result = IntervalSolver::solve(parse, mode, maxIter);
    for (const auto &line : result.log) {
        logger.log(QString::fromStdString(line));
    }
    if (!result.ok) {
        setStatus(QString::fromStdString(result.error), true);
        return;
    }

    ui->leftOutput->setText(QString::fromStdString(result.left));
    ui->rightOutput->setText(QString::fromStdString(result.right));
    ui->widthOutput->setText(QString::fromStdString(result.width));
    setStatus("OK.");
}
