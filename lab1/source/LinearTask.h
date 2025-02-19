#pragma once
#include <map>

#include "utility/Matrix.h"

#include "SlackFormTask.h"

/**
 * @brief Класс для представления задачи линейного программирования в стандартной форме
 * @details Представление задачи ЛП в виде: максимизировать f = c * x + v при условиях Ax <= b, x >= 0
 */
class LinearTask {
    std::size_t n;
    std::vector<double> c;
    double v;
    Matrix A;
    std::vector<double> b;

    /// x = x+ - x-, если нет ограничения x > 0. Переменная хранит список пар индексов x+, x-
    std::map<std::size_t, std::size_t> splitVarIndices;

    void removeEqualities(Matrix const& B, std::vector<double> const& b2);
    void addVarRestrictions(std::vector<std::size_t> const &notRestrictedVarIndices);
public:
    LinearTask(Matrix const &A, Matrix const &B, std::vector<double> const &b1, std::vector<double> const &b2, std::vector<double> const &c, double v,
               std::vector<std::size_t> const &notRestrictedVarIndices = {});

    std::vector<double> getTrueValues(std::vector<double> const &x);

    std::vector<double> const &getC() const { return c; }
    double getV() const { return v; }
    Matrix const &getA() const { return A; }
    std::vector<double> const &getB() const { return b; }
};
