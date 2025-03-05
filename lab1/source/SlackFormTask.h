#pragma once

#include "StandardTask.h"
#include "utility/custom_types.h"
#include "utility/matrix_functions.h"

const double EPS = 1e-9;

class SlackFormTask {
    std::size_t n, m;
    Matrix<double> tableau;

    std::vector<std::size_t> basis;

    double freeTerm; /// Постоянная в целевой фукнции

    void pivot(int& row, int& col);

    static std::size_t find(std::vector<std::size_t> const& v, std::size_t value);
    std::pair<std::size_t, double> findMaxCoef();
public:
    /**
     * @brief Конструктор получает задачу ЛП в стандартной форме и преобразует её в каноническую форму
     */
    SlackFormTask(StandardTask const& task);

    double simplex(std::vector<double>& solution);

    void print();
};