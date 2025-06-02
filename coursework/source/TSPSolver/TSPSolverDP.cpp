#include "TSPSolverDP.hpp"

#include <algorithm>

void TSPSolverDP::solve() {
    const int n = graph.adj.size();
    const int INF = std::numeric_limits<int>::max();

    // dp[mask][i] = минимальная стоимость пути в i, пройдя вершины в mask
    std::vector<std::vector<int>> dp(1 << n, std::vector<int>(n, INF));
    std::vector<std::vector<int>> parent(1 << n, std::vector<int>(n, -1));  // Для восстановления пути

    dp[1][0] = 0; // Стартуем из вершины 0

    for (int mask = 1; mask < (1 << n); ++mask) {
        for (int u = 0; u < n; ++u) {
            if (!(mask & (1 << u))) continue; // u не в маске
            if (dp[mask][u] == INF) continue;
            for (int v = 0; v < n; ++v) {
                if (mask & (1 << v)) continue; // уже посещён
                if (graph.adj[u][v] == INF) continue;

                int next_mask = mask | (1 << v);
                int new_cost = dp[mask][u] + graph.adj[u][v];
                if (new_cost < dp[next_mask][v]) {
                    dp[next_mask][v] = new_cost;
                    parent[next_mask][v] = u;
                }
            }
        }
    }

    // Завершаем путь, возвращаясь в 0
    int res = INF;
    int last_node = -1;
    int final_mask = (1 << n) - 1;

    for (int i = 1; i < n; ++i) {
        if (graph.adj[i][0] == INF || dp[final_mask][i] == INF) continue;

        int cost = dp[final_mask][i] + graph.adj[i][0];
        if (cost < res) {
            res = cost;
            last_node = i;
        }
    }

    if (res == INF) return;

    // Восстановление маршрута
    std::vector<int> path;
    int mask = final_mask;
    int current = last_node;

    while (current != -1) {
        path.push_back(current);
        int temp = parent[mask][current];
        mask ^= (1 << current);
        current = temp;
    }

    path.push_back(0); // возвращение в начальную вершину
    std::reverse(path.begin(), path.end());

    bestCost = res;
    bestPath = path;
}
