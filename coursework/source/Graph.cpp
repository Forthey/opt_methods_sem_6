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

Graph Graph::generateRandomConnected(int N) {
    Graph g(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> weightDist(1, 100);

    std::vector<int> vertices(N);
    std::iota(vertices.begin(), vertices.end(), 0);
    std::shuffle(vertices.begin(), vertices.end(), gen);

    // Строим простое остовное дерево: соединяем i-ю вершину с случайной предыдущей
    for (int i = 1; i < N; i++) {
        int v = vertices[i];
        int u = vertices[gen() % i];
        int w = weightDist(gen);
        g.adj[v][u] = w;
        g.adj[u][v] = w;
    }
    // Дополнительные случайные рёбра
    std::uniform_real_distribution<> prob(0.0, 1.0);
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            if (g.adj[i][j] >= std::numeric_limits<int>::max() / 2) {
                if (prob(gen) < 0.3) { // добавляем ребро с вероятностью 0.3
                    int w = weightDist(gen);
                    g.adj[i][j] = w;
                    g.adj[j][i] = w;
                }
            }
        }
    }
    for (int i = 0; i < N; i++) {
        g.adj[i][i] = 0;
    }
    return g;
}