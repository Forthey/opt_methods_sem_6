#pragma once

#include <vector>
#include <limits>

#include "Graph.hpp"          // Класс Graph с матрицей смежности

/**
 * @brief Абстрактный базовый класс для всех алгоритмов решения задачи коммивояжёра.
 *
 * Задаёт единый интерфейс:
 *  - solve() — запуск алгоритма поиска
 *  - getBestCost() — возвращает стоимость лучшего найденного пути
 *  - getBestPath() — возвращает вектор вершин, задающий этот путь
 *
 * Хранит ссылку на граф, поля для хранения лучшего решения и его стоимости.
 * Соответствует принципу Open/Closed (новые алгоритмы добавляются через наследование),
 * Single Responsibility (не содержит логики конкретного алгоритма),
 * Dependency Inversion (зависит от абстракции Graph).
 */
class TSPSolver {
protected:
    /// Ссылка на граф (матрица смежности)
    const Graph& graph;

    /// Стоимость лучшего найденного маршрута
    int bestCost;

    /// Сам маршрут: последовательность вершин, включая возврат в стартовую
    std::vector<int> bestPath;

public:
    /**
     * @brief Конструктор.
     * @param g Ссылка на объект Graph, содержащий матрицу смежности.
     * Инициализирует bestCost максимальным значением int.
     */
    explicit TSPSolver(const Graph& g)
        : graph(g), bestCost(std::numeric_limits<int>::max()) {}

    /// Виртуальный деструктор, необходим для корректного уничтожения в иерархии наследников
    virtual ~TSPSolver() = default;

    /**
     * @brief Запустить алгоритм поиска оптимального пути.
     * Должен быть реализован в подклассах.
     */
    virtual void solve() = 0;

    /**
     * @brief Получить стоимость лучшего найденного пути.
     * @return Стоимость маршрута (int). Если solve() ещё не вызывался — max int.
     */
    int getBestCost() const {
        return bestCost;
    }

    /**
     * @brief Получить сам маршрут (последовательность вершин).
     * @return Вектор вершин: начиная с 0, посещённые вершины, и возврат в 0.
     */
    const std::vector<int>& getBestPath() const {
        return bestPath;
    }
};
