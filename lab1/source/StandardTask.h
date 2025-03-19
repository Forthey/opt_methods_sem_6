#pragma once

#include <map>
#include <set>

#include "utility/custom_types.h"
#include "Task.h"

/**
 * @brief Класс для представления задачи линейного программирования в стандартной форме
 * @details Представление задачи ЛП в виде: максимизировать f = c * x + v при условиях Ax <= b, x >= 0
 */
class StandardTask {
    std::size_t n;
    Vector<double> targetFunction;
    bool targetFunctionSignChanged = false;
    double freeTerm;
    Matrix<double> A;
    Vector<double> b;

    /// x = x+ - x-, если нет ограничения x > 0. Переменная хранит список пар индексов x+, x-
    std::map<std::size_t, std::size_t> splitVarIndices;
    std::set<std::size_t> changedSignIndices;
public:
    explicit StandardTask(Task const& task);

    std::vector<double> getTrueValues(std::vector<double> const &x) const;

    std::size_t getDim() const { return n; }

    Vector<double> const& getTargetFunction() const {
        return targetFunction;
    }

    double getFreeTerm() const { return freeTerm; }

    Matrix<double> const& getA() const { return A; }

    Vector<double> const& getB() const { return b; }

    void print();
};
