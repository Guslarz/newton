#pragma once

#include <QMainWindow>

#include "datainput.h"
#include "newtonoutput.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

signals:
    void parsed(const Result*);

private:
    DataInput *dataInput;
    NewtonOutput *newtonOutput;
    QTabWidget *tabWidget;

    void setContent();
    void setMenu();
    void selectImportFile();
    void selectExportFile();
    void showError(int);
    void showResult(const Result*);

private slots:
    void intervalArithmeticResult(const std::vector<std::array<std::string, 4>>*, const std::array<std::string, 2>*);
    void floatingPointResult(const std::vector<std::array<std::string, 4>>*, const std::array<std::string, 2>*);
};
