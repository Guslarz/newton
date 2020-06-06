#include "intervaloutput.h"


IntervalOutput::IntervalOutput(const QString &label_text, const repr_t &repr,
                               QWidget *parent) : QLabel(parent)
{
    setWordWrap(true);
    if (repr[1].empty())
        setText(label_text + " = " + QString::fromStdString(repr[0]));
    else setText(label_text + " ∈ [" + QString::fromStdString(repr[0]) +
                        ", " + QString::fromStdString(repr[1]) + "]\n szerokość przedziału = " + QString::fromStdString(repr[2]));
    setTextInteractionFlags(Qt::TextSelectableByMouse);
}
