#pragma once

#include <vector>
#include <limits>

#include "custom_types.h"


template<typename T>
Matrix<T> transform(Matrix<T> const &matr) {
    Matrix transformed(matr[0].size(), matr.size());

    for (std::size_t i = 0; i < matr.size(); i++) {
        for (std::size_t j = 0; j < matr[0].size(); j++) {
            transformed[j][i] = matr[i][j];
        }
    }

    return transformed;
}

template<typename T>
std::size_t getRank(Matrix<T> const& m) {
    Matrix<T> matr = m;


    std::size_t rank = 0; // Счетчик ненулевых (ведущих) строк
    for (std::size_t col = 0, row = 0; col < matr[0].size() && row < matr.size(); ++col) {
        // Поиск строки с максимальным элементом по модулю в текущем столбце
        std::size_t pivot = row;
        for (std::size_t i = row; i < matr.size(); i++) {
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
        for (std::size_t j = col; j < matr[0].size(); j++)
            matr[row][j] /= pivotVal;

        // Обнуление всех элементов в текущем столбце для остальных строк
        for (std::size_t i = 0; i < matr.size(); i++) {
            if (i != row && std::abs(matr[i][col]) > std::numeric_limits<double>::epsilon()) {
                double factor = matr[i][col];
                for (std::size_t j = col; j < matr[0].size(); j++) {
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