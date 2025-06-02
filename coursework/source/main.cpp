#include <format>
#include <iostream>
#include <limits>
#include <fstream>

#include <nlohmann/json.hpp>

#include "TSPSolver/TSPSolverBrute.hpp"
#include "TSPSolver/TSPSolverDP.hpp"
#include "TSPSolver/TSPSolverGenetic.hpp"

#include "TimeWatcher.h"


struct Result {
    struct AlgResult {
        std::string name;
        int min_cost;
        double est_calc_time_seconds;

        nlohmann::json toJSON() const {
            return nlohmann::json{
                {"name", name},
                {"min_cost", min_cost},
                {"est_calc_time_seconds", est_calc_time_seconds}
            };
        }
    };

    int graph_size;
    std::vector<AlgResult> results;

    nlohmann::json toJSON() const {
        auto j = nlohmann::json{
            {"graph_size", graph_size},
            {"results", nlohmann::json::array()},
        };

        for (auto &result: results) {
            j["results"].push_back(result.toJSON());
        }

        return j;
    }
};


void printAdjMatrix(Graph const &g) {
    // Выводим матрицу смежности (0 значит отсутствующее ребро или диагональ)
    std::cout << "Adjacent matrix:" << std::endl;
    for (int i = 0; i < g.N; i++) {
        for (int j = 0; j < g.N; j++) {
            int w = (g.adj[i][j] >= std::numeric_limits<int>::max() / 2 ? 0 : g.adj[i][j]);
            std::cout << w << "\t";
        }
        std::cout << std::endl;
    }
}

void saveResultsToFile(std::vector<Result> const &results, std::string const &filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
        return;
    }

    nlohmann::json j;

    for (auto &result: results) {
        j.push_back(result.toJSON());
    }

    file << j.dump(4);
}

std::vector<Graph> generateGraphs(int minSize, int maxSize) {
    std::vector<Graph> graphs;

    for (int i = minSize; i <= maxSize; i++) {
        graphs.push_back(Graph::generateSymmetricConnectedGraph(i));
    }

    return graphs;
}


int main() {
    std::vector<Result> results;
    TimeWatcher timeWatcher;

    int minSize, maxSize, sizeStep;
    std::cout << "Enter min, max and step of graph sizes, separated by space\n>";

    std::cin >> minSize >> maxSize >> sizeStep;

    if (maxSize < minSize) {
        std::swap(minSize, maxSize);
    }

    auto graphs = generateGraphs(minSize, maxSize);

    for (auto& graph : graphs) {
        Result result;
        result.graph_size = graph.N;

        std::cout << "n = " << graph.N << std::endl;

        std::unordered_map<std::string, std::shared_ptr<TSPSolver> > solvers{
            {"brute_force", std::make_shared<TSPSolverBrute>(graph)},
            {"held_karp", std::make_shared<TSPSolverDP>(graph)},
            {"genetic", std::make_shared<TSPSolverGenetic>(graph)}
        };

        for (auto &[name, solver]: solvers) {
            if (name == "brute_force" and graph.N > 11) {
                continue;
            }
            if (name == "held_karp" and graph.N > 22) {
                continue;
            }

            timeWatcher.start();
            solver->solve();
            timeWatcher.stop();

            result.results.push_back(Result::AlgResult{
                name,
                solver->getBestCost(),
                timeWatcher.getLastElapsedSeconds()
            });

            std::cout << name << ": min cost = " << solver->getBestCost() << ", path = ";
            for (int v: solver->getBestPath()) std::cout << v << " ";
            std::cout << std::endl;
        }

        results.push_back(result);
    }

    saveResultsToFile(results, "results.json");

    return 0;
}
