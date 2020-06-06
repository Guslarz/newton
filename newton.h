#pragma once


#include "intervals.h"
#include <string>
#include <vector>
#include <iostream>


using iarithm_t = Interval<long double>;
using fparithm_t = float;


struct Result {
    repr_t x, y;
    std::vector<repr_t> xi, ai;
};


std::ostream& operator<<(std::ostream&, const Result&);


template <class T>
struct Comparator {
    bool operator()(const std::pair<const std::array<T, 2>&, T>&, const std::pair<const std::array<T, 2>&, T>&) const;
};


/*
 * 0 - OK
 * 1 - Zero div
 * 2 - interval lower > upper
 * 3 - interval invalid value
 * 4 - no data
 * 5 - interval in fparithm
 */
template <class T>
int newton(Result&, const T&, const std::vector<std::array<T, 2>>&);
int newtonIA(Result&, const std::array<std::string, 2>&, const std::vector<std::array<std::string, 4>>&);
int newtonFPA(Result&, const std::array<std::string, 2>&, const std::vector<std::array<std::string, 4>>&);
template <class T>
repr_t getRepr(const T&);
template <class T>
bool containsZero(const T&);


template <class T>
int newton(Result &result, const T &x, const std::vector<std::array<T, 2>> &data)
{
    Comparator<T> comparator;
    std::vector<std::pair<std::array<T, 2>, T>> sorted;
    for (auto &value : data)
        sorted.push_back(std::make_pair(value, abs(value[0] - x)));
    std::sort(sorted.begin(), sorted.end(), comparator);

    std::vector<T> ai, xi;
    for (auto &value : sorted) {
        xi.push_back(value.first[0]);
        ai.push_back(value.first[1]);
    }

    size_t end = sorted.size() - 1;
    for (size_t i = 0; i != end; ++i) {
        for (size_t j = end; j != i; --j) {
            T tmp = (xi[j] - xi[j - i - 1]);
            if (containsZero<T>(tmp))
                return 1;
            ai[j] = (ai[j] - ai[j - 1]) / tmp;
        }
    }

    std::size_t i = ai.size();
    T y(ai[--i]);
    while (i-- > 0) {
        y *= x - xi[i];
        y += ai[i];
    }

    result.xi.clear();
    result.ai.clear();
    result.x = getRepr<T>(x);
    result.y = getRepr<T>(y);
    for (auto &value : xi)
        result.xi.push_back(getRepr<T>(value));
    result.xi.pop_back();
    for (auto &value : ai)
        result.ai.push_back(getRepr<T>(value));

    return 0;
}
