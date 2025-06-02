#include "TSPSolverDP.hpp"

#include <algorithm>

void TSPSolverDP::solve() {
    int N = graph.N;
    int M = 1 << (N - 1);
    std::vector<std::vector<int> > dp(M, std::vector<int>(N - 1, std::numeric_limits<int>::max()));
    std::vector<std::vector<int> > parent(M, std::vector<int>(N - 1, -1));

    // Начальные случаи: из 0 в каждую i
    for (int i = 0; i < N - 1; i++) {
        if (graph.adj[0][i + 1] < std::numeric_limits<int>::max() / 2) {
            dp[1 << i][i] = graph.adj[0][i + 1];
            parent[1 << i][i] = 0;
        }
    }

    // Основной DP: растём по размеру множества mask
    for (int mask = 0; mask < M; mask++) {
        for (int j = 0; j < N - 1; j++) {
            if (!(mask & (1 << j))) continue;
            int costJ = dp[mask][j];
            if (costJ >= std::numeric_limits<int>::max() / 2) continue;
            for (int k = 0; k < N - 1; k++) {
                if (mask & (1 << k)) continue;
                int newMask = mask | (1 << k);
                int newCost = costJ + graph.adj[j + 1][k + 1];
                if (newCost < dp[newMask][k]) {
                    dp[newMask][k] = newCost;
                    parent[newMask][k] = j + 1;
                }
            }
        }
    }

    // Завершаем цикл, возвращаясь из j+1 в 0
    bestCost = std::numeric_limits<int>::max();
    int fullMask = M - 1;
    for (int j = 0; j < N - 1; j++) {
        if (dp[fullMask][j] < std::numeric_limits<int>::max() / 2 && graph.adj[j + 1][0] < std::numeric_limits<
                int>::max() / 2) {
            int totalCost = dp[fullMask][j] + graph.adj[j + 1][0];
            if (totalCost < bestCost) {
                bestCost = totalCost;
                // Восстановление пути
                bestPath = {0};
                int cur = j;
                int mask = fullMask;
                while (mask) {
                    bestPath.push_back(cur + 1);
                    int prev = parent[mask][cur];
                    mask ^= (1 << cur);
                    if (prev == 0) break;
                    cur = prev - 1;
                }
                std::reverse(bestPath.begin() + 1, bestPath.end());
                bestPath.push_back(0);
            }
        }
    }
}
