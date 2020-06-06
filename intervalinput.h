#pragma once

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include "intervals.h"

class IntervalInput : public QWidget
{
    Q_OBJECT
public:
    explicit IntervalInput(const QString&, QWidget *parent = nullptr);
    std::array<std::string, 2> getValue() const;
    QWidget* getLower() const {return inputLower;}
    QWidget* getUpper() const {return inputUpper;}

    void setValue(const QString&, const QString&);

private:
    QComboBox *select;
    QLineEdit *inputLower, *inputUpper;
};
