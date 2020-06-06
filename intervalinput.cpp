#include "intervalinput.h"

#include <QHBoxLayout>
#include <QLabel>


IntervalInput::IntervalInput(const QString &label_text, QWidget *parent) : QWidget(parent)
{
    setFixedWidth(800);

    auto *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(15);

    auto *label = new QLabel(label_text, this);
    label->setFixedWidth(50);
    layout->addWidget(label);

    select = new QComboBox(this);
    QStringList options = {"Wartość", "Przedział"};
    select->addItems(options);
    layout->addWidget(select);

    inputLower = new QLineEdit(this);
    layout->addWidget(inputLower);

    inputUpper = new QLineEdit(this);
    layout->addWidget(inputUpper);
    inputUpper->hide();

    connect(select, QOverload<int>::of(&QComboBox::activated), [=](int index) {
        switch(index) {
            case 0:
                inputUpper->setText("");
                inputUpper->hide();
                break;
            case 1:
                if (inputUpper->text() == "")
                    inputUpper->setText(inputLower->text());
                inputUpper->show();
                break;
        }
    });
}


std::array<std::string, 2> IntervalInput::getValue() const
{
    auto lower = inputLower->text().toStdString(),
            upper = inputUpper->text().toStdString();
    return {lower, upper.empty() ? lower : upper};
}


void IntervalInput::setValue(const QString &lower_str, const QString &upper_str)
{
    if (lower_str == upper_str) {
        select->setCurrentIndex(0);
        select->activated(0);
        inputLower->setText(lower_str);
    } else {
        select->setCurrentIndex(1);
        select->activated(1);
        inputLower->setText(lower_str);
        inputUpper->setText(upper_str);
    }
}
