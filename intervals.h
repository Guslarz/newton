#pragma once


#include <string>
#include <exception>
#include <cfenv>
#pragma fenv_access(on)
#include <algorithm>
#include <array>
#include <sstream>
#include <memory>


using repr_t = std::array<std::string, 3>;


template <class T>
class Interval
{
public:
    class ZeroDiv {};
    class WrongValue {};

private:
    T lower, upper;

public:
    Interval();
    Interval(const Interval&);
    Interval(const T&);
    Interval(const T&, const T&);
    Interval(const std::string&);
    Interval(const std::string&, const std::string&, T(*f)(const std::string&, std::size_t*) = std::stold);

    Interval& operator=(const Interval&);

    Interval& operator+=(const Interval&);
    Interval operator+(const Interval&) const;

    Interval& operator-=(const Interval&);
    Interval operator-(const Interval&) const;

    Interval& operator*=(const Interval&);
    Interval operator*(const Interval&) const;

    Interval& operator/=(const Interval&);
    Interval operator/(const Interval&) const;

    friend Interval<float> abs(const Interval<float>&);
    friend Interval<double> abs(const Interval<double>&);
    friend Interval<long double> abs(const Interval<long double>&);

    const T& getLower() const { return lower; }
    const T& getUpper() const { return upper; }
    repr_t getRepresentation() const;

    bool containsZero() const { return lower <= 0.0 && upper >= 0.0; }

private:
    Interval<T> toAbs() const;

    static void removeZeros(std::string&);
};


Interval<float> abs(const Interval<float>&);
Interval<double> abs(const Interval<double>&);
Interval<long double> abs(const Interval<long double>&);


template <class T>
Interval<T>::Interval()
{}


template <class T>
Interval<T>::Interval(const Interval<T> &other) :
    lower(other.lower), upper(other.upper)
{}


template <class T>
Interval<T>::Interval(const T &val) :
    lower(val), upper(val)
{}


template <class T>
Interval<T>::Interval(const T &lower, const T &upper) :
    lower(lower), upper(upper)
{
    if (lower > upper)
        throw WrongValue();
}


template<class T>
Interval<T>::Interval(const std::string &str) :
    Interval(str, str)
{}


template<class T>
Interval<T>::Interval(const std::string &lower_str, const std::string &upper_str, T(*str_read)(const std::string&, std::size_t*))
{
    fesetround(FE_DOWNWARD);
    lower = str_read(lower_str, nullptr);
    fesetround(FE_UPWARD);
    upper = str_read(upper_str, nullptr);
    fesetround(FE_TONEAREST);
    if (lower > upper) throw WrongValue();
}


template <class T>
Interval<T>& Interval<T>::operator=(const Interval<T> &other)
{
    lower = other.lower;
    upper = other.upper;
    return *this;
}


template <class T>
Interval<T>& Interval<T>::operator+=(const Interval<T> &other)
{
    fesetround(FE_DOWNWARD);
    lower += other.lower;
    fesetround(FE_UPWARD);
    upper += other.upper;
    fesetround(FE_TONEAREST);
    return *this;
}


template <class T>
Interval<T> Interval<T>::operator+(const Interval<T> &other) const
{
    Interval<T> tmp(*this);
    tmp += other;
    return tmp;
}


template <class T>
Interval<T>& Interval<T>::operator-=(const Interval<T> &other)
{
    fesetround(FE_DOWNWARD);
    lower -= other.upper;
    fesetround(FE_UPWARD);
    upper -= other.lower;
    fesetround(FE_TONEAREST);
    return *this;
}


template <class T>
Interval<T> Interval<T>::operator-(const Interval<T> &other) const
{
    Interval<T> tmp(*this);
    tmp -= other;
    return tmp;
}


template <class T>
Interval<T>& Interval<T>::operator*=(const Interval<T> &other)
{
    fesetround(FE_DOWNWARD);
    std::array<T, 4> down = {
        lower * other.lower,
        lower * other.upper,
        upper * other.lower,
        upper * other.upper
    };
    fesetround(FE_UPWARD);
    std::array<T, 4> up = {
        lower * other.lower,
        lower * other.upper,
        upper * other.lower,
        upper * other.upper
    };
    lower = *std::min_element(down.begin(), down.end());
    upper = *std::max_element(up.begin(), up.end());
    fesetround(FE_TONEAREST);
    return *this;
}


template <class T>
Interval<T> Interval<T>::operator*(const Interval<T> &other) const
{
    Interval<T> tmp(*this);
    tmp *= other;
    return tmp;
}


template <class T>
Interval<T>& Interval<T>::operator/=(const Interval<T> &other)
{
    if (other.containsZero())
        throw ZeroDiv();


    fesetround(FE_DOWNWARD);
    std::array<T, 4> down = {
        lower / other.lower,
        lower / other.upper,
        upper / other.lower,
        upper / other.upper
    };
    fesetround(FE_UPWARD);
    std::array<T, 4> up = {
        lower / other.lower,
        lower / other.upper,
        upper / other.lower,
        upper / other.upper
    };
    lower = *std::min_element(down.begin(), down.end());
    upper = *std::max_element(up.begin(), up.end());
    fesetround(FE_TONEAREST);
    return *this;
}


template <class T>
Interval<T> Interval<T>::operator/(const Interval<T> &other) const
{
    Interval<T> tmp(*this);
    tmp /= other;
    return tmp;
}


template <class T>
Interval<T> Interval<T>::toAbs() const
{
    auto bounds = std::minmax<T>(abs(lower), abs(upper));
    return Interval<T>(containsZero() ? static_cast<T>(0) : bounds.first, bounds.second);
}


template <class T>
repr_t Interval<T>::getRepresentation() const
{
    if (lower == upper) {
        std::ostringstream ss;
        ss.precision(10);
        ss.setf(std::ios::scientific);
        ss << lower;

        std::string value = ss.str();
        removeZeros(value);
        return {value, "", ""};
    }
    else {
        std::streamsize precision = 10;
        std::string lower_str, upper_str;

        while (true) {
            std::ostringstream ss_lower, ss_upper;
            ss_lower.setf(std::ios::scientific);
            ss_upper.setf(std::ios::scientific);
            ss_lower.precision(precision);
            ss_upper.precision(precision);

            fesetround(FE_DOWNWARD);
            ss_lower << lower;
            fesetround(FE_UPWARD);
            ss_upper << upper;

            lower_str = ss_lower.str();
            upper_str = ss_upper.str();

            if (lower_str == upper_str)
                precision += 10;
            else break;
        }
        fesetround(FE_TONEAREST);

        removeZeros(lower_str);
        removeZeros(upper_str);

        std::ostringstream ss_width;
        ss_width.setf(std::ios::scientific);
        ss_width.precision(3);
        fesetround(FE_UPWARD);
        ss_width << (upper - lower);

        return {lower_str, upper_str, ss_width.str()};
    }
}


template <class T>
void Interval<T>::removeZeros(std::string &str)
{
    std::size_t i = str.size(), exp, first_zero;
    while (str[--i] != 'e');
    exp = i;
    while (str[--i] == '0');
    if (str[i] == '.') i += 2;
    else i += 1;
    first_zero = i;
    str.replace(first_zero, exp - first_zero, "");
}
