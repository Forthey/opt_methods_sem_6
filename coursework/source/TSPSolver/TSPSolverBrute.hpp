#pragma once

#include "TSPSolver.hpp"


// Решение TSP полным перебором (фиксируем старт 0, перебираем перестановки [1..N-1])
class TSPSolverBrute : public TSPSolver {
    public:
        explicit TSPSolverBrute(const Graph& g) : TSPSolver(g) {}
    
        // solve(): генерируем все маршруты, вычисляем стоимость каждого, выбираем минимальный.
        void solve() override;
    };
    