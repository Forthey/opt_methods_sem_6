#pragma once

#include <random>

#include "TSPSolver.hpp"


// Генетический алгоритм для TSP: селекция (турнир), кроссовер (Order Crossover), мутация (swap).
class TSPSolverGenetic : public TSPSolver {
    public:
        int populationSize, generations;
        double crossoverProb, mutationProb;
        std::mt19937 gen;
    
        TSPSolverGenetic(const Graph& g, int popSize = 100, int gens = 500,
                         double crossProb = 0.8, double mutProb = 0.1)
            : TSPSolver(g), populationSize(popSize),
              generations(gens), crossoverProb(crossProb), mutationProb(mutProb) {
            if (g.N < 20) {
                popSize = 100;
            } else if (g.N < 50) {
                popSize = 200;
            } else {
                popSize = 500;
            }
            std::random_device rd; gen.seed(rd());
        }
    
        void solve() override;
    };
    