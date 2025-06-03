#pragma once

#include <random>

#include "TSPSolver.hpp"


// Генетический алгоритм для TSP: селекция (турнир), кроссовер (Order Crossover), мутация (swap).
class TSPSolverGenetic : public TSPSolver {
    public:
        int populationSize, generations;
        double crossoverProb, mutationProb;
        std::mt19937 gen;
    
        TSPSolverGenetic(const Graph& g, int popSize = -1, int gens = 100,
                         double crossProb = 0.8, double mutProb = 0.1)
            : TSPSolver(g), populationSize(popSize),
              generations(gens), crossoverProb(crossProb), mutationProb(mutProb) {
            std::random_device rd; gen.seed(rd());
            validateValues();
        }

        void validateValues() {
            if (populationSize == -1) {
                double const C = 50;

                populationSize = C * graph.N * std::log(graph.N + 1);
            }
        }
    
        void solve() override;
    };
    