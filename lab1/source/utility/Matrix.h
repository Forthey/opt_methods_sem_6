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

    std::size_t getRank() const {
        std::vector<std::vector<double>> matr = data;


        std::size_t rank = 0; // Счетчик ненулевых (ведущих) строк
        for (std::size_t col = 0, row = 0; col < getCols() && row < getRows(); ++col) {
            // Поиск строки с максимальным элементом по модулю в текущем столбце
            std::size_t pivot = row;
            for (std::size_t i = row; i < getRows(); i++) {
                if (std::abs(matr[i][col]) > std::abs(matr[pivot][col]))
                    pivot = i;
            }

            // Если опорный элемент близок к нулю, столбец пропускаем
            if (std::abs(matr[pivot][col]) < std::numeric_limits<double>::epsilon()) {
                continue;
            }

            // Обмен текущей строки с найденной строкой с опорным элементом
            std::swap(matr[row], matr[pivot]);

            // Нормализация текущей строки, чтобы опорный элемент стал равен 1
            double pivotVal = matr[row][col];
            for (std::size_t j = col; j < getCols(); j++)
                matr[row][j] /= pivotVal;

            // Обнуление всех элементов в текущем столбце для остальных строк
            for (std::size_t i = 0; i < getRows(); i++) {
                if (i != row && std::abs(matr[i][col]) > std::numeric_limits<double>::epsilon()) {
                    double factor = matr[i][col];
                    for (std::size_t j = col; j < getCols(); j++) {
                        matr[i][j] -= factor * matr[row][j];
                    }
                }
            }

            // Переход к следующей строке и увеличиваем счетчик ранга
            row++;
            rank++;
        }
        return rank;
    }

    Matrix transform() const {
        Matrix matr(getCols(), getRows());

        for (std::size_t i = 0; i < getRows(); i++) {
            for (std::size_t j = 0; j < getCols(); j++) {
                matr[j][i] = data[i][j];
            }
        }

        return matr;
    }
};
