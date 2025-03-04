//
// Created by 0Fort on 18.02.2025.
//

#include "SlackFormTask.h"

#include <iostream>
#include <memory>

/**
 *
 * @param e Индекс вводимой переменной из множества N (e в книге)
 * @param l Индекс выводимой переменной из множества B (l в книге)
 */
void SlackFormTask::pivot(std::size_t& e, std::size_t& l) {
    // Вычисление коэффициентов уравнения для новой базисной переменной Xe
    b[e] = b[l] / A[l][e];
    for (std::size_t j : N) {
        if (j == e) {
            continue;
        }
        A[e][j] = A[l][j] / A[l][e];
    }
    A[e][l] = 1 / A[l][e];

    // Вычисление коэффициентов остальных ограничений
    for (std::size_t i = 0; i < N.size(); i++) {
        if (i == l) {
            continue;
        }
        b[i] = b[i] - A[i][e] * b[e];
        for (std::size_t j : N) {
            if (j == e) {
                continue;
            }
            A[i][j] = A[i][j] - A[i][e] * A[e][j];
        }
        A[i][l] = -A[i][e] * A[e][l];
    }

    // Вычисление целевой функции
    v = v + c[e] * b[e];
    for (std::size_t j : N) {
        c[j] = c[j] - c[e] * A[e][j];
    }
    c[l] = -c[e] * A[e][l];

    // Обновляем базис: переменная, входящая в базис, заменяет ту, что покидает его
    std::swap(B[l], N[e]);
}

std::vector<double> SlackFormTask::simplex() {
    while (true) {
        std::size_t index = 0;
        double max = 0;

        for (std::size_t j = 0; j < N.size(); j++) {
            if (c[N[j]] > max) {
                index = j;
                max = c[N[j]];
            }
        }

        if (max <= std::numeric_limits<double>::epsilon()) {
            break;
        }

        double delta = std::numeric_limits<double>::infinity();
        std::size_t l;
        // std::size_t e = N[index];

        for (int i = 0; i < B.size(); i++) {
            if (A[i][index] > std::numeric_limits<double>::epsilon()) {
                double newDelta = b[i] / A[i][index];
                if (newDelta < delta) {
                    delta = newDelta;
                    l = i;
                }
            }
        }

        if (delta == std::numeric_limits<double>::infinity()) {
            throw std::runtime_error("CanonicalFormTask::simplex(): Задача неограниченна");
        }

        for (int i = 0; i < B.size(); i++) {
            std::cout << A[i][B[i]] << std::endl;
        }

        pivot(index, l);
    }

    std::vector<double> x(n);
    for (int i = 0; i < n; i++) {
        if (std::find(B.begin(), B.end(), i) != B.end()) {
            x[i] = b[i];
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

SlackFormTask::SlackFormTask(Matrix const &A, std::vector<double> const &b, std::vector<double> const &c, double v)
    : v(v), A(b.size(), c.size() + b.size()), m(b.size()), n(c.size()), b(b), c(c.size() + b.size(), 0.0) {

    // Заполняем строки ограничений
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            this->A[i][j] = A[i][j];
        }
    }

    // Добавляем слэк-переменные (единичная матрица)
    for (int i = 0; i < m; i++) {
        this->A[i][n+i] = 1;
    }

    // Строка целевой функции: для задачи максимизации запишем c
    for (int j = 0; j < n; j++) {
        this->c[j] = c[j];
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


