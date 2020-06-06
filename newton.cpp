#include "newton.h"
#include <iostream>


std::ostream& operator<<(std::ostream &out, const Result &result)
{
    out << "x: " << result.x[0] << " " << result.x[1] << " " << result.x[2] << "\n";
    out << "y: " << result.y[0] << " " << result.y[1] << " " << result.y[2] << "\n";
    for (size_t i = 0; i < result.ai.size(); ++i)
        out << "a" << i << ": " << result.ai[i][0] << " " << result.ai[i][1] << " " << result.ai[i][2] << "\n";
    for (size_t i = 0; i < result.xi.size(); ++i)
        out << "x" << i << ": " << result.xi[i][0] << " " << result.xi[i][1] << " " << result.xi[i][2] << "\n";
    return out;
}


template <>
bool Comparator<iarithm_t>::operator()(const std::pair<const std::array<iarithm_t, 2>&, iarithm_t> &lhs,
                                       const std::pair<const std::array<iarithm_t, 2>&, iarithm_t> &rhs) const
{
    return lhs.second.getLower() < rhs.second.getLower();
}


template <>
bool Comparator<fparithm_t>::operator()(const std::pair<const std::array<fparithm_t, 2>&, fparithm_t> &lhs,
                                        const std::pair<const std::array<fparithm_t, 2>&, fparithm_t> &rhs) const
{
    return lhs.second < rhs.second;
}


template <>
repr_t getRepr(const iarithm_t &value)
{
    return value.getRepresentation();
}


template <>
repr_t getRepr(const fparithm_t &value)
{
    return {std::to_string(value), "", ""};
}


template <>
bool containsZero(const iarithm_t &value)
{
    return value.containsZero();
}


template <>
bool containsZero(const fparithm_t &value)
{
    return value == 0;
}


int newtonIA(Result &result, const std::array<std::string, 2> &x, const std::vector<std::array<std::string, 4>> &data)
{
    if (data.size() == 0)
        return 4;

    try {
        iarithm_t parsedX(x[0], x[1]);
        std::vector<std::array<iarithm_t, 2>> parsedData;
        for (auto &value : data)
            parsedData.push_back({
                iarithm_t(value[0], value[1]),
                iarithm_t(value[2], value[3])
            });
        return newton<iarithm_t>(result, parsedX, parsedData);
    } catch (iarithm_t::WrongValue&) {
        return 2;
    } catch (std::exception&) {
        return 3;
    }
}


int newtonFPA(Result &result, const std::array<std::string, 2> &x, const std::vector<std::array<std::string, 4>> &data)
{
    if (data.size() == 0)
        return 4;

    try {
        if (x[0] != x[1])
            return 5;
        fparithm_t parsedX = static_cast<fparithm_t>(std::stold(x[0]));
        std::vector<std::array<fparithm_t, 2>> parsedData;
        for (auto &value : data) {
            if (value[0] != value[1] || value[2] != value[3])
                return 5;
            parsedData.push_back({
                static_cast<fparithm_t>(std::stold(value[0])),
                static_cast<fparithm_t>(std::stold(value[2]))
            });
        }
        return newton<fparithm_t>(result, parsedX, parsedData);
    } catch (std::exception&) {
        return 3;
    }
}
