#pragma once
#include <array>
#include <string>
#include <cstdint>


template<std::uint8_t size = 2>
using RealVector = std::array<double, size>;

template<std::uint8_t size = 2>
static std::string RealVectorToString(RealVector<size> const& x) {
    std::string result = "(";

    for (std::size_t i = 0; i < x.size(); ++i) {
        if (i == x.size() - 1) {
            result += std::to_string(x[i]) + ")";
        } else {
            result += std::to_string(x[i]) + ", ";
        }
    }

    return result;
}