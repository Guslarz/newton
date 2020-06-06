#include "mainwindow.h"

#include <QScrollArea>
#include <QScrollBar>
#include <QMessageBox>
#include <QMenuBar>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    dataInput = new DataInput(this);
    newtonOutput = new NewtonOutput(this);

    setWindowTitle("Wielomian Newtona");
    setFixedHeight(600);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    setContent();
    setMenu();

    setFixedWidth(sizeHint().width());
}


void MainWindow::setContent()
{
    auto *content = new QWidget(this);
    tabWidget = new QTabWidget(this);

    auto *dataInputScrollArea = new QScrollArea(tabWidget),
         *newtonOutputScrollArea = new QScrollArea(tabWidget);
    dataInputScrollArea->setWidget(dataInput);
    dataInputScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    dataInputScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    dataInputScrollArea->setMinimumWidth(dataInput->minimumSizeHint().width() +
                                         dataInputScrollArea->verticalScrollBar()->sizeHint().width());
    newtonOutputScrollArea->setWidget(newtonOutput);
    newtonOutputScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    newtonOutputScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    newtonOutputScrollArea->setMinimumWidth(newtonOutput->minimumSizeHint().width() +
                                            newtonOutputScrollArea->verticalScrollBar()->sizeHint().width());

    tabWidget->addTab(dataInputScrollArea, "Wartości");
    tabWidget->addTab(newtonOutputScrollArea, "Wielomian");

    auto *contentLayout = new QVBoxLayout(content);
    auto *titleLabel = new QLabel("OBLICZANIE WARTOŚĆI I WSPÓŁCZYNNIKÓW WIELOMIANU INTERPOLACYJNEGO NEWTONA", content);
    contentLayout->addWidget(titleLabel);
    contentLayout->addWidget(tabWidget);

    setCentralWidget(content);

    connect(dataInput, SIGNAL (submitIA(const std::vector<std::array<std::string, 4>>*, const std::array<std::string, 2>*)),
            this, SLOT (intervalArithmeticResult(const std::vector<std::array<std::string, 4>>*, const std::array<std::string, 2>*)));
    connect(dataInput, SIGNAL (submitFP(const std::vector<std::array<std::string, 4>>*, const std::array<std::string, 2>*)),
            this, SLOT (floatingPointResult(const std::vector<std::array<std::string, 4>>*, const std::array<std::string, 2>*)));
    connect(this, SIGNAL (parsed(const Result*)),
            newtonOutput, SLOT (showResult(const Result*)));
}


void MainWindow::setMenu()
{
    auto *fileMenu = menuBar()->addMenu(tr("&Plik"));

    auto *newAct = new QAction(tr("&Nowy"), this);
    connect(newAct, &QAction::triggered, dataInput, &DataInput::newFile);
    fileMenu->addAction(newAct);

    auto *importAct = new QAction(tr("&Importuj"));
    connect(importAct, &QAction::triggered, this, &MainWindow::selectImportFile);
    fileMenu->addAction(importAct);

    auto *exportAct = new QAction(tr("&Eksportuj"));
    connect(exportAct, &QAction::triggered, this, &MainWindow::selectExportFile);
    fileMenu->addAction(exportAct);
}


void MainWindow::showError(int error)
{
    newtonOutput->setNoResult();
    switch (error) {
    case 1:
        QMessageBox::warning(this, "Błąd dzialenia przez 0", "Wartości x<sub>i</sub> nie mogą się pokrywać");
        break;
    case 2:
        QMessageBox::warning(this, "Błędne dane wejściowe", "Podano przedział, w którym pierwsza wartość jest większa od drugiej");
        break;
    case 3:
        QMessageBox::warning(this, "Błędne dane wejściowe", "Wśród danych wykryto wartość niebędącą liczbą");
        break;
    case 4:
        QMessageBox::warning(this, "Błąd", "Brak danych wejściowych");
        break;
    case 5:
        QMessageBox::warning(this, "Błąd", "Podano przedział, a wybrano obliczenia w arytmetyce zmiennoprzecinkowej");
        break;
    }
}


void MainWindow::showResult(const Result *result)
{
    emit parsed(result);
    tabWidget->setCurrentIndex(1);
}


void MainWindow::intervalArithmeticResult(const std::vector<std::array<std::string, 4>> *data,
                                          const std::array<std::string, 2> *x)
{
    Result *result = new Result;
    int error = newtonIA(*result, *x, *data);

    if (error) {
        delete result;
        showError(error);
    } else showResult(result);
}


void MainWindow::floatingPointResult(const std::vector<std::array<std::string, 4>> *data,
                                          const std::array<std::string, 2> *x)
{
    Result *result = new Result;
    int error = newtonFPA(*result, *x, *data);

    if (error) {
        delete result;
        showError(error);
    } else showResult(result);
}


void MainWindow::selectImportFile()
{
    auto filename = QFileDialog::getOpenFileName(this, "Wybierz plik do importu", "", "Wielomian Newtona - Dane (*.newton)");
    if (!filename.isEmpty())
        dataInput->importFile(filename);
}


void MainWindow::selectExportFile()
{
    auto filename = QFileDialog::getSaveFileName(this, "Wybierz nazwę dla eksportowanego pliku", "", "Wielomian Newtona - Dane (*.newton)");
    if (!filename.isEmpty())
        dataInput->exportFile(filename);
}
