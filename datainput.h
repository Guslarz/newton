#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "intervalinput.h"

#include <vector>
#include <array>


class DataInput : public QWidget
{
    Q_OBJECT
public:
    explicit DataInput(QWidget *parent = nullptr);

    void newFile();
    void importFile(const QString&);
    void exportFile(const QString&);

signals:
    void submitIA(const std::vector<std::array<std::string, 4>>*, const std::array<std::string, 2>*);
    void submitFP(const std::vector<std::array<std::string, 4>>*, const std::array<std::string, 2>*);

private:
    QVBoxLayout *inputsLayout;
    QPushButton *removeButton;
    std::vector<IntervalInput*> inputs;
    IntervalInput *xInput;

    void clear();

private slots:
    void addRow();
    void removeRow();
    void onSubmitIA();
    void onSubmitFP();
};
