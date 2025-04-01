#pragma once
#include <array>
#include <string>
#include <cstdint>

#include "RealVector.h"


template<std::uint8_t size = 2>
class RealMatrix {
    std::array<RealVector<size>, size> data;

public:
    template<
        typename... Values,
        typename = std::enable_if_t<(std::is_same_v<Values, RealVector<size>> && ...)> >
    explicit RealMatrix(Values... x) : data{x...} {
        static_assert(sizeof...(Values) == size, "Incorrect number of arguments!");
    }

    RealMatrix() = default;

    RealVector<size> const &operator[](std::size_t const &i) const {
        return data[i];
    }

    RealVector<size> &operator[](std::size_t const &i) {
        return data[i];
    }

    RealVector<size> operator*(RealVector<size> const &v) const {
        RealVector<size> result;

        for (std::uint8_t i = 0; i < size; ++i) {
            result[i] = 0.0;
            for (std::uint8_t j = 0; j < size; ++j) {
                result[i] += data[i][j] * v[j];
            }
        }

        return result;
    }
};
