#pragma once

#include "StandardTask.h"
#include "utility/custom_types.h"
#include "utility/matrix_functions.h"


class SlackFormTask {
    std::size_t n, m;
    /// Таблица симплекс-метода: m строк для ограничений + 1 строка для целевой функции.
    /// Всего столбцов: n (исходных переменных) + m (слэк-переменных) + 1 (правые части)
    Matrix<double> A;
    std::vector<double> b;
    std::vector<double> targetFunction;
    std::vector<std::size_t> N; /// Множество индексов небазисных переменных
    std::vector<std::size_t> B; /// Множество индексов базисных переменных
    double freeTerm; /// Постоянная в целевой фукнции

    void pivot(std::size_t& e, std::size_t& l);

    static std::size_t find(std::vector<std::size_t> const& v, std::size_t value);
    std::pair<std::size_t, double> findMaxCoef();
public:
    /**
     * @brief Конструктор получает задачу ЛП в стандартной форме и преобразует её в каноническую форму
     */
    SlackFormTask(StandardTask const& task);

    std::vector<double> simplex();

    void print();
};