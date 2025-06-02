#include <iostream>
#include <limits>

#include "TSPSolver/TSPSolverBrute.hpp"
#include "TSPSolver/TSPSolverDP.hpp"
#include "TSPSolver/TSPSolverGenetic.hpp"

#pragma execution_character_set( "utf-8" )

int main() {
    // Генерируем случайный связный граф на 10 вершинах и сохраняем в JSON
    Graph g = Graph::generateRandomConnected(3);
    g.saveToJson("random_graph.json");

    // Выводим матрицу смежности (0 значит отсутствующее ребро или диагональ)
    std::cout << "Матрица смежности графа:" << std::endl;
    for (int i = 0; i < g.N; i++) {
        for (int j = 0; j < g.N; j++) {
            int w = (g.adj[i][j] >= std::numeric_limits<int>::max() / 2 ? 0 : g.adj[i][j]);
            std::cout << w << "\t";
        }
        std::cout << std::endl;
    }

    // Алгоритм полного перебора
    TSPSolverBrute solverBrute(g);
    solverBrute.solve();
    std::cout << "Brute Force: min cost = " << solverBrute.getBestCost() << ", path = ";
    for (int v : solverBrute.getBestPath()) std::cout << v << " ";
    std::cout << std::endl;

    // Алгоритм Беллмана-Хелда-Карпа (DP)
    TSPSolverDP solverDP(g);
    solverDP.solve();
    std::cout << "DP Held-Karp: min cost = " << solverDP.getBestCost() << ", path = ";
    for (int v : solverDP.getBestPath()) std::cout << v << " ";
    std::cout << std::endl;

    // Генетический алгоритм
    TSPSolverGenetic solverGA(g, 50, 200, 0.8, 0.1);
    solverGA.solve();
    std::cout << "Genetic: min cost = " << solverGA.getBestCost() << ", path = ";
    for (int v : solverGA.getBestPath()) std::cout << v << " ";
    std::cout << std::endl;

    return 0;
}
