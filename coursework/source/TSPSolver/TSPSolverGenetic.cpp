#include "TSPSolverGenetic.hpp"

#include <algorithm>


void TSPSolverGenetic::solve() {
    int N = graph.N;
    if (N <= 1) return;
    int len = N - 1; // длина хромосомы (вершины 1..N-1)
    // Создаём начальную популяцию случайных туров
    std::vector<std::vector<int> > population(populationSize);
    std::vector<int> base(len);
    for (int i = 0; i < len; i++) base[i] = i + 1;
    for (int i = 0; i < populationSize; i++) {
        population[i] = base;
        std::shuffle(population[i].begin(), population[i].end(), gen);
    }

    bestCost = std::numeric_limits<int>::max();
    bestPath.clear();

    // Функция вычисления стоимости тура (включая возвращение в 0)
    auto tourCost = [&](const std::vector<int> &tour) {
        int cost = 0, prev = 0;
        for (int v: tour) {
            if (graph.adj[prev][v] >= std::numeric_limits<int>::max() / 2) return std::numeric_limits<int>::max();
            cost += graph.adj[prev][v];
            prev = v;
        }
        if (graph.adj[prev][0] >= std::numeric_limits<int>::max() / 2) return std::numeric_limits<int>::max();
        cost += graph.adj[prev][0];
        return cost;
    };

    std::uniform_real_distribution<> distProb(0.0, 1.0);
    std::uniform_int_distribution<> distPop(0, populationSize - 1);

    // Основной цикл эволюции
    for (int genIdx = 0; genIdx < generations; genIdx++) {
        // Оценка: вычисляем стоимости туров
        std::vector<int> costs(populationSize);
        for (int i = 0; i < populationSize; i++) {
            costs[i] = tourCost(population[i]);
            if (costs[i] < bestCost) {
                bestCost = costs[i];
                bestPath = {0};
                for (int v: population[i]) bestPath.push_back(v);
                bestPath.push_back(0);
            }
        }

        // Новая популяция (с элитаризмом)
        std::vector<std::vector<int> > newPop;
        int bestIndex = std::min_element(costs.begin(), costs.end()) - costs.begin();
        newPop.push_back(population[bestIndex]); // сохраняем лучшую особь

        // Функция турнира: выбираем k случайных и возвращаем лучшую по стоимости
        auto tournamentSelect = [&](int k = 5) {
            int bestIdx = distPop(gen), bestCostSel = costs[bestIdx];
            for (int i = 1; i < k; i++) {
                int idx = distPop(gen);
                if (costs[idx] < bestCostSel) {
                    bestIdx = idx;
                    bestCostSel = costs[idx];
                }
            }
            return population[bestIdx];
        };

        // Пока не заполнили новую популяцию
        while ((int) newPop.size() < populationSize) {
            auto parent1 = tournamentSelect();
            auto parent2 = tournamentSelect();
            std::vector<int> child1(len, -1), child2(len, -1);

            // Кроссовер (Order Crossover)
            if (distProb(gen) < crossoverProb) {
                std::uniform_int_distribution<> distPoint(0, len - 1);
                int a = distPoint(gen), b = distPoint(gen);
                if (a > b) std::swap(a, b);
                // Копируем отрезок [a..b] из родителей
                std::vector<bool> inSeg(N, false);
                for (int i = a; i <= b; i++) {
                    child1[i] = parent1[i];
                    child2[i] = parent2[i];
                    inSeg[parent1[i]] = true;
                }
                // Заполняем оставшиеся позиции у потомков
                int idx1 = (b + 1) % len;
                for (int i = 0; i < len; i++) {
                    int v = parent2[(b + 1 + i) % len];
                    if (!inSeg[v]) {
                        child1[idx1] = v;
                        idx1 = (idx1 + 1) % len;
                    }
                }
                std::fill(inSeg.begin(), inSeg.end(), false);
                for (int i = a; i <= b; i++) inSeg[parent2[i]] = true;
                int idx2 = (b + 1) % len;
                for (int i = 0; i < len; i++) {
                    int v = parent1[(b + 1 + i) % len];
                    if (!inSeg[v]) {
                        child2[idx2] = v;
                        idx2 = (idx2 + 1) % len;
                    }
                }
            } else {
                child1 = parent1;
                child2 = parent2;
            }

            // Мутация swap
            auto swapMutate = [&](std::vector<int> &chr) {
                if (distProb(gen) < mutationProb) {
                    std::uniform_int_distribution<> distIdx(0, len - 1);
                    int i = distIdx(gen), j = distIdx(gen);
                    std::swap(chr[i], chr[j]);
                }
            };
            swapMutate(child1);
            swapMutate(child2);

            newPop.push_back(child1);
            if ((int) newPop.size() < populationSize) newPop.push_back(child2);
        }
        population = std::move(newPop);
    }
}
