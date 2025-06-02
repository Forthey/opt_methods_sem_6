#include "TSPSolverBrute.hpp"

#include <algorithm>


void TSPSolverBrute::solve() {
    int N = graph.N;
    std::vector<int> vertices;
    for (int i = 1; i < N; i++) {
        vertices.push_back(i);
    }
    bestCost = std::numeric_limits<int>::max();
    bestPath.clear();
    do {
        int cost = 0;
        int prev = 0;
        bool valid = true;
        // проходим по маршруту 0 -> vertices -> 0
        for (int v: vertices) {
            if (graph.adj[prev][v] >= std::numeric_limits<int>::max() / 2) {
                valid = false;
                break;
            }
            cost += graph.adj[prev][v];
            prev = v;
        }
        if (valid) {
            if (graph.adj[prev][0] >= std::numeric_limits<int>::max() / 2) valid = false;
            else cost += graph.adj[prev][0];
        }
        if (valid && cost < bestCost) {
            bestCost = cost;
            bestPath.clear();
            bestPath.push_back(0);
            for (int v: vertices) bestPath.push_back(v);
            bestPath.push_back(0);
        }
    } while (std::next_permutation(vertices.begin(), vertices.end()));
}
