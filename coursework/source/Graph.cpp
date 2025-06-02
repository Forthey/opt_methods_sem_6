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

Graph Graph::generateHamiltonianGraph(int n, int extra_edges, int max_weight) {
    Graph graph(n);

    std::vector<std::vector<int>> adj(n, std::vector<int>(n, std::numeric_limits<int>::max()));

    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<int> weight_dist(1, max_weight);

    // Полный граф
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            adj[i][j] = adj[j][i] = weight_dist(rng);
        }
    }

    // // 1. Создаём случайный гамильтонов цикл
    // std::vector<int> nodes(n);
    // for (int i = 0; i < n; ++i) nodes[i] = i;
    // std::shuffle(nodes.begin(), nodes.end(), rng);
    //
    // for (int i = 0; i < n; ++i) {
    //     int u = nodes[i];
    //     int v = nodes[(i + 1) % n];
    //     int w = weight_dist(rng);
    //     adj[u][v] = adj[v][u] = w;
    // }
    //
    // // 2. Добавляем случайные дополнительные рёбра
    // std::uniform_int_distribution<int> node_dist(0, n - 1);
    // int added = 0;
    // while (added < extra_edges) {
    //     int u = node_dist(rng);
    //     int v = node_dist(rng);
    //     if (u == v || adj[u][v] != std::numeric_limits<int>::max()) continue;
    //
    //     int w = weight_dist(rng);
    //     adj[u][v] = adj[v][u] = w;
    //     ++added;
    // }

    graph.adj = adj;

    return graph;
}