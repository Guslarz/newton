#include "datainput.h"

#include <QGridLayout>
#include <QMessageBox>
#include <fstream>


DataInput::DataInput(QWidget *parent) : QWidget(parent)
{
    inputsLayout = new QVBoxLayout(this);
    inputsLayout->setSizeConstraint(QLayout::SetFixedSize);

    auto *lastRow = new QWidget(this);
    auto *lastRowLayout = new QGridLayout(lastRow);
    lastRowLayout->setMargin(0);
    auto *addButton = new QPushButton("Dodaj", lastRow),
         *submitIAButton = new QPushButton("Oblicz w arytmetyce przedziałowej", lastRow),
         *submitFPButton = new QPushButton("Oblicz w arytmetyce zmiennoprzecinkowej", lastRow);
    removeButton = new QPushButton("Usuń", lastRow);
    xInput = new IntervalInput("x", lastRow);
    lastRowLayout->addWidget(addButton, 0, 0, 1, 1);
    lastRowLayout->addWidget(removeButton, 0, 1, 1, 1);
    lastRowLayout->addWidget(xInput, 1, 0, 1, 2);
    lastRowLayout->addWidget(submitIAButton, 2, 0, 1, 1);
    lastRowLayout->addWidget(submitFPButton, 2, 1, 1, 1);
    lastRowLayout->setRowMinimumHeight(1, 50);

    addRow();
    inputsLayout->addWidget(lastRow);
    removeButton->setDisabled(true);

    connect(addButton, SIGNAL (clicked()), this, SLOT (addRow()));
    connect(removeButton, SIGNAL (clicked()), this, SLOT (removeRow()));
    connect(submitIAButton, SIGNAL (clicked()), this, SLOT (onSubmitIA()));
    connect(submitFPButton, SIGNAL (clicked()), this, SLOT (onSubmitFP()));
}


void DataInput::newFile()
{
    clear();
    addRow();
}


void DataInput::importFile(const QString &filename)
{
    std::fstream in(filename.toStdString(), std::ios::in);
    if (in.fail()) {
        QMessageBox::warning(this, "Błąd", "Otwarcie pliku nie powiodło się");
        return;
    }

    clear();

    std::array<std::string, 4> buf;
    size_t i = 0, count = 0;

    while (std::getline(in, buf[i])) {
        if (i == 3) {
            addRow();
            inputs[count * 2]->setValue(QString::fromStdString(buf[0]), QString::fromStdString(buf[1]));
            inputs[count * 2 + 1]->setValue(QString::fromStdString(buf[2]), QString::fromStdString(buf[3]));
            i = 0;
            ++count;
        } else ++i;
    }

    if (i == 2)
        xInput->setValue(QString::fromStdString(buf[0]), QString::fromStdString(buf[1]));
    else
        QMessageBox::warning(this, "Błąd importu", "Niepoprawny format pliku");

    in.close();
}


void DataInput::exportFile(const QString &filename)
{
    std::fstream out(filename.toStdString() + (filename.right(7) != ".newton" ? ".newton" : ""), std::ios::out);
    if (out.fail()) {
        QMessageBox::warning(this, "Błąd", "Zapis do pliku nie powiódł się");
        return;
    }

    for (const auto &input : inputs) {
        auto values = input->getValue();
        out << values[0] << "\n" << values[1] << "\n";
    }

    auto values = xInput->getValue();
    out << values[0]<< "\n" << values[1] << std::endl;

    out.close();
}


void DataInput::addRow()
{
    auto x_label = QString("x<sub>%1</sub>").arg(QString::number(inputs.size() / 2)),
            y_label = QString("P(%1)").arg(x_label);

    auto *input = new IntervalInput(x_label);
    inputsLayout->insertWidget(static_cast<int>(inputs.size()), input);
    if (!inputs.empty())
        setTabOrder(inputs.back()->getLower(), input->getLower());
    inputs.push_back(input);
    input = new IntervalInput(y_label);
    inputsLayout->insertWidget(static_cast<int>(inputs.size()), input);
    setTabOrder(inputs.back(), input);
    setTabOrder(inputs.back()->getLower(), input->getLower());
    inputs.push_back(input);

    removeButton->setDisabled(false);
}


void DataInput::removeRow()
{
    switch (inputs.size()) {
        case 2:
            return;
        case 4:
            removeButton->setDisabled(true);
    }

    auto *input = inputs.back();
    inputsLayout->removeWidget(input);
    inputs.pop_back();
    delete input;

    input = inputs.back();
    inputsLayout->removeWidget(input);
    inputs.pop_back();
    delete input;
}


void DataInput::onSubmitIA()
{
    auto *data = new std::vector<std::array<std::string, 4>>();
    for (auto it = inputs.begin(); it != inputs.end(); it += 2) {
        auto x = (*it)->getValue(),
             y = (*(it + 1))->getValue();
        if (!(x[0].empty() && y[0].empty()))
            data->push_back({x[0], x[1], y[0], y[1]});
    }
    emit submitIA(data, new std::array<std::string, 2>(xInput->getValue()));
}


void DataInput::onSubmitFP()
{
    auto *data = new std::vector<std::array<std::string, 4>>();
    for (auto it = inputs.begin(); it != inputs.end(); it += 2) {
        auto x = (*it)->getValue(),
             y = (*(it + 1))->getValue();
        if (!(x[0].empty() && y[0].empty()))
            data->push_back({x[0], x[1], y[0], y[1]});
    }
    emit submitFP(data, new std::array<std::string, 2>(xInput->getValue()));
}


void DataInput::clear()
{
    IntervalInput *input;
    while (inputs.size()) {
        input = inputs.back();
        inputsLayout->removeWidget(input);
        inputs.pop_back();
        delete input;
    }
}
