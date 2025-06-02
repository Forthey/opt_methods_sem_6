#pragma once

#include "TSPSolver.hpp"


// Решение TSP динамическим программированием (Held-Karp)
class TSPSolverDP : public TSPSolver {
    public:
        explicit TSPSolverDP(const Graph& g) : TSPSolver(g) {}
    
        // solve(): битовое DP. Вычисляем dp[mask][i] для всех подмножеств mask и заканчивающей вершины i+1.
        void solve() override;
    };
    