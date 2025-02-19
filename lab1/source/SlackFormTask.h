#pragma once

#include "utility/Matrix.h"


class SlackFormTask {
    std::size_t n, m;
    /// Таблица симплекс-метода: m строк для ограничений + 1 строка для целевой функции.
    /// Всего столбцов: n (исходных переменных) + m (слэк-переменных) + 1 (правые части)
    std::vector<std::vector<double>> D;
    std::vector<std::size_t> N; /// Множество индексов небазисных переменных
    std::vector<std::size_t> B; /// Множество индексов базисных переменных
    double v; /// Постоянная в целевой фукнции

    void pivot(std::size_t& e, std::size_t& l);

    static std::size_t find(std::vector<std::size_t> const& v, std::size_t value);
    std::pair<std::size_t, double> findMaxCoef();
public:
    /**
     * @brief Конструктор получает задачу ЛП в стандартной форме и преобразует её в каноническую форму
     */
    SlackFormTask(Matrix const& A, std::vector<double> const& b, std::vector<double> const& c, double v);

    std::vector<double> simplex();
};