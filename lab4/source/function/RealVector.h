#pragma once
#include <array>
#include <string>
#include <cstdint>
#include <sstream>
#include <cmath>


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

    explicit RealVector(double value = 0.0) {
        for (std::size_t i = 0; i < size; ++i) {
            data[i] = value;
        }
    }

    [[nodiscard]] std::string toString(double const epsilon = 0.00001) const {
        std::string result = "(";

        for (std::uint8_t i = 0; i < size; ++i) {
            if (i == size - 1) {
                result += toStringWithPrecision(data[i], epsilon) + ")";
            } else {
                result += toStringWithPrecision(data[i], epsilon) + ", ";
            }
        }

        return result;
    }

    static std::string toStringWithPrecision(double const value, double epsilon) {
        int precision = 0;
        while (epsilon < 1.0) {
            epsilon *= 10.0;
            precision++;
        }
        ++precision;

        std::ostringstream out;
        out.precision(precision);
        out << std::fixed << value;

        return std::move(out).str();
    }

    double normOfDifference(RealVector const &x) {
        double result = 0.0;

        for (std::uint8_t i = 0; i < size; ++i) {
            result += (data[i] - x[i]) * (data[i] - x[i]);
        }

        return std::sqrt(result);
    }

    double norm() {
        double result = 0.0;

        for (std::uint8_t i = 0; i < size; ++i) {
            result += data[i] * data[i];
        }

        return std::sqrt(result);
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

    RealVector operator*(double value) {
        RealVector result;

        for (std::uint8_t i = 0; i < size; ++i) {
            result[i] = data[i] * value;
        }

        return result;
    }

    double operator*(RealVector const& v) {
        double result = 0.0;

        for (std::uint8_t i = 0; i < size; ++i) {
            result += data[i] * v[i];
        }

        return result;
    }
};
