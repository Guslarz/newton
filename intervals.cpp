#include "intervals.h"


Interval<float> abs(const Interval<float> &interval)
{
    return interval.toAbs();
}


Interval<double> abs(const Interval<double> &interval)
{
    return interval.toAbs();
}


Interval<long double> abs(const Interval<long double> &interval)
{
    return interval.toAbs();
}
