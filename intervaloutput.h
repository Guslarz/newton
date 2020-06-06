#pragma once

#include <QLabel>

#include "newton.h"


class IntervalOutput : public QLabel
{
    Q_OBJECT
public:
    explicit IntervalOutput(const QString&, const repr_t&, QWidget *parent = nullptr);
};
