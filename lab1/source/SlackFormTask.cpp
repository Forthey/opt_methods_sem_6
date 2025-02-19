//
// Created by 0Fort on 18.02.2025.
//

#include "SlackFormTask.h"

#include <memory>

/**
 *
 * @param e Индекс вводимой переменной из множества N (e в книге)
 * @param l Индекс выводимой переменной из множества B (l в книге)
 */
void SlackFormTask::pivot(std::size_t& e, std::size_t& l) {
    // double pivotVal = D[l][e];
    // // Делим строку l на опорный элемент, чтобы получить единицу в позиции (l, e)
    // for (int j = 0; j < D[l].size(); j++) {
    //     D[l][j] /= pivotVal;
    // }
    // // Обновляем все остальные строки: обнуляем столбец e
    // for (int i = 0; i < D.size(); i++) {
    //     if (i == l) {
    //         continue;
    //     }
    //     double factor = D[i][e];
    //     for (int j = 0; j < D[0].size(); j++) {
    //         D[i][j] -= factor * D[l][j];
    //     }
    // }

    // Вычисление коэффициентов уравнения для новой базисной переменной Xe
    D[e].back() = D[l].back() / D[l][e];
    for (std::size_t j : N) {
        if (j == e) {
            continue;
        }
        D[e][j] = D[l][j] / D[l][e];
    }
    D[e][l] = 1 / D[l][e];

    // Вычисление коэффициентов остальных ограничений
    for (std::size_t i = 0; i < N.size(); i++) {
        if (i == l) {
            continue;
        }
        D[i].back() = D[i].back() - D[i][e] * D[e].back();
        for (std::size_t j : N) {
            if (j == e) {
                continue;
            }
            D[i][j] = D[i][j] - D[i][e] * D[e][j];
        }
        D[i][l] = -D[i][e] * D[e][l];
    }

    // Вычисление целевой функции
    v = v + D.back()[e] * D[e].back();
    for (std::size_t j : N) {
        D.back()[j] = D.back()[j] - D.back()[e] * D.back()[j] * D[e][j];
    }
    D.back()[l] = D.back()[e] * D[e][l];

    // Обновляем базис: переменная, входящая в базис, заменяет ту, что покидает его
    std::swap(B[l], N[e]);
}

std::vector<double> SlackFormTask::simplex() {
    while (true) {
        std::size_t index = 0;
        double max = 0;

        for (std::size_t j = 0; j < N.size(); j++) {
            if (D.back()[N[j]] > max) {
                index = j;
                max = D.back()[N[j]];
            }
        }

        if (max <= std::numeric_limits<double>::epsilon()) {
            break;
        }

        double delta = std::numeric_limits<double>::infinity();
        std::size_t l;
        std::size_t e = N[index];

        for (int i = 0; i < B.size(); i++) {
            if (D[i][e] > std::numeric_limits<double>::epsilon()) {
                double newDelta = D[i].back() / D[i][e];
                if (newDelta < delta) {
                    delta = newDelta;
                    l = i;
                }
            }
        }

        if (delta == std::numeric_limits<double>::infinity()) {
            throw std::runtime_error("CanonicalFormTask::simplex(): Задача неограниченна");
        }
        pivot(e, l);
    }

    std::vector<double> x(n);
    for (int i = 0; i < n; i++) {
        if (std::find(B.begin(), B.end(), i) != B.end()) {
            x[i] = D[i].back();
        } else {
            x[i] = 0;
        }
    }

    return x;
}

std::size_t SlackFormTask::find(std::vector<std::size_t> const &v, std::size_t value) {
    std::size_t result = 0;
    while (result < v.size() && v[result] != value) {
        ++result;
    }

    if (result == v.size()) {
        throw std::runtime_error("CanonicalFormTask::pivot: index out of range");
    }

    return result;
}

SlackFormTask::SlackFormTask(Matrix const &A, std::vector<double> const &b, std::vector<double> const &c, double v) : v(v) {
    m = b.size();
    n = c.size();
    // Инициализируем таблицу: (m + 1) строк, (n + m + 1) столбцов
    D = std::vector(m+1, std::vector(n+m+1, 0.0));

    // Заполняем строки ограничений
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            D[i][j] = A[i][j];
        }
    }
    // Добавляем слэк-переменные (единичная матрица)
    for (int i = 0; i < m; i++) {
        D[i][n+i] = 1;
    }
    // Записываем правые части ограничений
    for (int i = 0; i < m; i++) {
        D[i][n+m] = b[i];
    }
    // Строка целевой функции: для задачи максимизации запишем c
    for (int j = 0; j < n; j++) {
        D[m][j] = c[j];
    }

    // Инициализируем базисные переменные - слэк-переменные (индексы n, n+1, …, n+m-1)
    // И не базисные: исходные переменные (индексы 0, 1, …, n-1)
    B.resize(m);
    N.resize(n);
    for (int i = 0; i < m; i++) {
        B[i] = n + i;
    }
    for (int j = 0; j < n; j++) {
        N[j] = j;
    }
}


