#include "Graph.hpp"

#include <iostream>
#include <fstream>
#include <random>

#include <nlohmann/json.hpp>


bool Graph::loadFromJson(const std::string &filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Не удалось открыть файл " << filename << std::endl;
        return false;
    }
    nlohmann::json j;
    inFile >> j;
    if (!j.contains("adjacency_matrix")) {
        std::cerr << "Неверный формат JSON: отсутствует ключ adjacency_matrix" << std::endl;
        return false;
    }
    auto matrix = j["adjacency_matrix"];
    N = matrix.size();
    adj.assign(N, std::vector(N, std::numeric_limits<int>::max()));
    for (int i = 0; i < N; i++) {
        if (!matrix[i].is_array() || matrix[i].size() != N) {
            std::cerr << "Неверный формат матрицы в JSON" << std::endl;
            return false;
        }
        for (int j = 0; j < N; j++) {
            int w = matrix[i][j];
            if (w != 0) {
                adj[i][j] = w; // если вес не нулевой, записываем его
            }
            if (i == j) adj[i][j] = 0; // диагональ = 0
        }
    }
    return true;
}

bool Graph::saveToJson(const std::string &filename) const {
    nlohmann::json j;
    j["adjacency_matrix"] = nlohmann::json::array();
    for (int i = 0; i < N; i++) {
        j["adjacency_matrix"].push_back(nlohmann::json::array());
        for (int j2 = 0; j2 < N; j2++) {
            int w = (adj[i][j2] >= std::numeric_limits<int>::max() / 2 ? 0 : adj[i][j2]);
            j["adjacency_matrix"][i].push_back(w);
        }
    }
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Не удалось открыть файл для записи " << filename << std::endl;
        return false;
    }
    outFile << j.dump(4);
    return true;
}

Graph Graph::generateSymmetricConnectedGraph(int n, int minWeight, int maxWeight) {
    Graph g(n);

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<int> dist(minWeight, maxWeight);

    std::vector<std::vector<int>> adjMatrix(n, std::vector<int>(n, 0));

    // Сначала строим связный граф в виде остовного дерева
    std::vector<bool> visited(n, false);
    visited[0] = true;

    for (int i = 1; i < n; ++i) {
        int from;
        do {
            from = rng() % i;  // выбираем случайную уже посещённую вершину
        } while (!visited[from]);

        int weight = dist(rng);
        adjMatrix[i][from] = adjMatrix[from][i] = weight;
        visited[i] = true;
    }

    // Добавляем дополнительные случайные рёбра для плотности
    int extraEdges = n;  // можно настроить плотность
    for (int k = 0; k < extraEdges; ++k) {
        int i = rng() % n;
        int j = rng() % n;
        if (i != j && adjMatrix[i][j] == 0) {
            int weight = dist(rng);
            adjMatrix[i][j] = adjMatrix[j][i] = weight;
        }
    }

    g.adj = std::move(adjMatrix);

    return g;
}