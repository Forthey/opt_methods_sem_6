#pragma once
#include <format>
#include <vector>


class Matrix {
    std::vector<std::vector<double>> data;

public:
    Matrix(Matrix const &m) = default;

    Matrix(Matrix &&m) noexcept : data(std::move(m.data)) {}

    Matrix &operator=(Matrix const &m) = default;

    Matrix(size_t rows, size_t cols) {
        for (size_t i = 0; i < rows; i++) {
            data.emplace_back(cols);
        }
    }

    explicit Matrix(std::initializer_list<std::vector<double>> const& data) : data{data} {
    }

    std::vector<double> &operator[](std::size_t i) { return data[i]; }
    std::vector<double> const &operator[](std::size_t i) const { return data[i]; }

    std::size_t getRows() const { return data.size(); }
    std::size_t getCols() const { return data.empty() ? 0 : data[0].size(); }

    void extendRight() {
        for (auto& v : data) {
            v.push_back(0.0);
        }
    }

    void extendDown(std::vector<double> const& v) {
        data.push_back(v);
    }

    decltype(data) const& getData() const { return data; }
    decltype(data)& getData() { return data; }
};
