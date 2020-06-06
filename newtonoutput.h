#ifndef NEWTONOUTPUT_H
#define NEWTONOUTPUT_H

#include <QWidget>
#include <QVBoxLayout>

#include "intervaloutput.h"


class NewtonOutput : public QWidget
{
    Q_OBJECT
public:
    explicit NewtonOutput(QWidget *parent = nullptr);

    void setNoResult();

private slots:
    void showResult(const Result*);

private:
    QVBoxLayout *layout;

    void clear();
};

#endif // NEWTONOUTPUT_H
