#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "core/Parser.h"
#include "core/ComputationLogger.h"
#include "core/CholeskySolver.h"
#include "core/IntervalSolver.h"
#include "interval/Interval.h"
#include <mpreal.h>
#include <sstream>
#include <iomanip>

using namespace mpfr;
using namespace interval_arithmetic;

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
QString formatScientificList(const std::string& input) {
    std::ostringstream oss;
    oss << "[";
    std::string current;
    bool first = true;
    for (char c : input) {
        if ((c >= '0' && c <= '9') || c == '-' || c == '+' || c == '.' || c == 'E' || c == 'e') {
            current += c;
        } else {
            if (!current.empty()) {
                double val = std::stod(current);
                if (!first)
                    oss << ", ";
                oss << std::scientific << val;
                first = false;
                current.clear();
            }
        }
    }
    if (!current.empty()) {
        double val = std::stod(current);
        if (!first)
            oss << ", ";
        oss << std::scientific << val;
    }
    oss << "]";
    return QString::fromStdString(oss.str());
}
std::vector<double> extractDoubles(const std::string& s) {
    std::vector<double> res;
    std::string num;
    bool in_num = false;
    for (char c : s) {
        if ((c >= '0' && c <= '9') || c == '.' || c == '-' || c == 'E' || c == 'e' || c == '+') {
            num += c;
            in_num = true;
        } else {
            if (in_num && !num.empty()) {
                res.push_back(std::stod(num));
                num.clear();
            }
            in_num = false;
        }
    }
    if (in_num && !num.empty()) {
        res.push_back(std::stod(num));
    }
    return res;
}

QString formatScientificListToFixed(const std::string& input) {
    std::ostringstream oss;
    oss << "[";
    std::string current;
    bool first = true;
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        // Złap cyfry, minusy, kropki, E/e, plusy (dla notacji naukowej)
        if ((c >= '0' && c <= '9') || c == '-' || c == '+' || c == '.' || c == 'E' || c == 'e') {
            current += c;
        } else {
            if (!current.empty()) {
                double val = std::stod(current);
                if (!first)
                    oss << ", ";
                oss << std::fixed << std::setprecision(17) << val;
                first = false;
                current.clear();
            }
        }
    }
    if (!current.empty()) {
        double val = std::stod(current);
        if (!first)
            oss << ", ";
        oss << std::fixed << std::setprecision(17) << val;
    }
    oss << "]";
    return QString::fromStdString(oss.str());
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
    ComputationLogger logger(ui->logOutput);
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


    if (ui->modeNormal->isChecked()) {
        std::vector<std::vector<double>> A;
        std::vector<double> b;
        QString error;
        if (!Parser::toDoubleMatrix(parse, A, b, error)) {
            setStatus(error, true);
            return;
        }

        CholeskyResult result = CholeskySolver::solve(A, b);
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
    IntervalResult result = IntervalSolver::solve(parse, mode);
    for (const auto &line : result.log) {
        logger.log(QString::fromStdString(line));
    }
    if (!result.ok) {
        setStatus(QString::fromStdString(result.error), true);
        return;
    }

    ui->leftOutput->setText(formatScientificListToFixed(result.left));
    ui->rightOutput->setText(formatScientificListToFixed(result.right));
    ui->widthOutput->setText(formatScientificList(result.width));
    setStatus("OK.");
}
