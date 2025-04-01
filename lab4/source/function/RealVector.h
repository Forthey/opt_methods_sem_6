#pragma once
#include <array>
#include <string>
#include <cstdint>


template<std::uint8_t size = 2>
class RealVector {
    std::array<double, size> data;

public:
    template<
        typename... Values,
        typename = std::enable_if_t<(std::is_same_v<Values, double> && ...)> >
    explicit RealVector(Values... x) : data{x...} {
        static_assert(sizeof...(Values) == size, "Incorrect number of arguments!");
    }

    RealVector() {
        for (std::size_t i = 0; i < size; ++i) {
            data[i] = 0;
        }
    }

    [[nodiscard]] std::string toString() const {
        std::string result = "(";

        for (std::uint8_t i = 0; i < size; ++i) {
            if (i == size - 1) {
                result += std::to_string(data[i]) + ")";
            } else {
                result += std::to_string(data[i]) + ", ";
            }
        }

        return result;
    }

    double normOfDifference(RealVector const &x) {
        double result = 0.0;

        for (std::uint8_t i = 0; i < size; ++i) {
            result = std::max(result, std::abs(data[i] - x[i]));
        }

        return result;
    }

    double& operator[](std::uint8_t index) {
        return data[index];
    }

    double operator[](std::uint8_t index) const {
        return data[index];
    }

    RealVector operator-(RealVector const &x) {
        RealVector result;

        for (std::uint8_t i = 0; i < size; ++i) {
            result[i] = data[i] - x[i];
        }

        return result;
    }

    RealVector operator-(double value) {
        RealVector result;

        for (std::uint8_t i = 0; i < size; ++i) {
            result[i] = data[i] - value;
        }

        return result;
    }

    RealVector operator*(double value) {
        RealVector result;

        for (std::uint8_t i = 0; i < size; ++i) {
            result[i] = data[i] * value;
        }

        return result;
    }
};
