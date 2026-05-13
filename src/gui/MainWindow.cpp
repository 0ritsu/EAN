#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "core/Parser.h"
#include "core/IterationLogger.h"

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

    ParseResult parse = Parser::parse(rawInput);
    if (!parse.ok) {
        setStatus(parse.error, true);
        return;
    }

    logger.log("[INFO] Wejście sparsowane poprawnie. TODO: uruchom solver.");
    setStatus("OK. TODO: obliczenia.");

    // TODO:
    // - odczytaj tryb (radio button)
    // - wywołaj solver (zwykły / przedziałowy)
    // - wypisz lewy/prawy koniec oraz szerokość
    // - loguj każdy krok iteracji
    // - obsłuż limit iteracji
}
