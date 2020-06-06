#include "newtonoutput.h"


NewtonOutput::NewtonOutput(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);
    setNoResult();
}


void NewtonOutput::showResult(const Result *result)
{
    clear();

    size_t i = result->ai.size();
    QLabel *out;

    out = new QLabel("Wzór:", this);
    layout->addWidget(out);
    QString formulae = QString("a<sub>%1</sub>").arg(i - 1);
    while(--i != 0)
        formulae = QString("(%1)").arg(formulae) + QString("(x - x<sub>%1</sub>) + a<sub>%1</sub>").arg(i - 1);
    formulae = "P(x) = " + formulae;
    out = new QLabel(formulae, this);
    out->setWordWrap(true);
    out->setFixedWidth(800);
    layout->addWidget(out);

    out = new QLabel("Wynik:", this);
    layout->addWidget(out);
    out = new IntervalOutput("P(x) ", result->y, this);
    layout->addWidget(out);
    out = new IntervalOutput("x ", result->x, this);
    layout->addWidget(out);

    out = new QLabel("Współczynniki:", this);
    layout->addWidget(out);
    for (size_t i = 0; i < result->ai.size(); ++i) {
        out = new IntervalOutput(QString("a<sub>%1</sub> ").arg(i), result->ai[i], this);
        layout->addWidget(out);
    }

    for (size_t i = 0; i < result->xi.size(); ++i) {
        out = new IntervalOutput(QString("x<sub>%1</sub> ").arg(i), result->xi[i], this);
        layout->addWidget(out);
    }

    delete result;
}


void NewtonOutput::setNoResult()
{
    clear();

    auto *label = new QLabel("Brak wyników", this);
    layout->addWidget(label);
}


void NewtonOutput::clear()
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}
