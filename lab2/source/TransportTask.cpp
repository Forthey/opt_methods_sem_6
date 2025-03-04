#include "TransportTask.h"

#include <fstream>
#include <iostream>

#include "utility/incorrect_format_exception.h"


void TransportTask::balanceTask() {
    std::int64_t supply = 0, demand = 0;
    for (std::int64_t supplier: suppliers) {
        supply += supplier;
    }
    for (std::int64_t consumer: consumers) {
        demand += consumer;
    }
    if (demand > supply) {
        fakeOne = Supplier;
        suppliers.emplace_back(demand - supply);
    } else if (demand < supply) {
        fakeOne = Consumer;
        consumers.emplace_back(supply - demand);
    }
}

TransportTask::TransportTask(std::string const &filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Невозможно отрыть файл: " << filename << std::endl;
        exit(1);
    }

    std::int64_t suppliersCnt, consumersCnt;

    if (!(file >> suppliersCnt >> consumersCnt)) {
        throw IncorrectFormatException(filename);
    }

    suppliers.resize(suppliersCnt);
    consumers.resize(consumersCnt);
    pathCosts.resize(suppliersCnt);

    for (std::int64_t i = 0; i < suppliersCnt; ++i) {
        if (!(file >> suppliers[i])) {
            throw IncorrectFormatException(filename);
        }
    }

    for (std::int64_t i = 0; i < consumersCnt; ++i) {
        if (!(file >> consumers[i])) {
            throw IncorrectFormatException(filename);
        }
    }

    for (std::int64_t i = 0; i < suppliersCnt; ++i) {
        pathCosts[i].resize(consumersCnt);
        for (std::int64_t j = 0; j < consumersCnt; ++j) {
            if (!(file >> pathCosts[i][j])) {
                throw IncorrectFormatException(filename);
            }
        }
    }

    balanceTask();
}

std::vector<std::vector<std::int64_t> > TransportTask::northwestCornerMethod() {
    std::vector basicPlan(pathCosts.size(), std::vector<std::int64_t>(pathCosts[0].size()));
    std::vector suppliers = this->suppliers, consumers = this->consumers;

    std::int64_t supIndex = 0, consIndex = 0;

    while (supIndex < suppliers.size() && consIndex < consumers.size()) {
        if (suppliers[supIndex] > consumers[consIndex]) {
            basicPlan[supIndex][consIndex] = consumers[consIndex];
            suppliers[supIndex] -= consumers[consIndex];
            consumers[consIndex] = 0;
            consIndex++;
        } else if (suppliers[supIndex] < consumers[consIndex]) {
            basicPlan[supIndex][consIndex] = suppliers[supIndex];
            consumers[consIndex] -= suppliers[supIndex];
            suppliers[supIndex] = 0;
            supIndex++;
        } else {
            basicPlan[supIndex][consIndex] = suppliers[supIndex];
            suppliers[supIndex] = 0;
            consumers[consIndex] = 0;
            consIndex++;
            supIndex++;
        }
    }

    return basicPlan;
}

std::vector<std::vector<std::int64_t> > TransportTask::potentialsMethod(
    std::vector<std::vector<std::int64_t> > &basicPlan) const {
    int m = pathCosts.size();
    int n = pathCosts[0].size();
    std::vector<double> u(m), v(n);

    std::vector isBasic(m, std::vector(n, false));

    // Помечаем базисные клетки
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (basicPlan[i][j] > 0) {
                isBasic[i][j] = true;
            }
        }
    }

    while (true) {
        // 1. Вычисляем потенциалы
        computePotentials(isBasic, u, v);

        // 2. Проверяем условие оптимальности: для всех не-базисных ячеек вычисляем дельту
        std::int64_t minDelta = 0;
        int enter_i = -1, enter_j = -1;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (!isBasic[i][j]) {
                    // не-базисная ячейка
                    std::int64_t delta = pathCosts[i][j] - (u[i] + v[j]);
                    if (delta < minDelta) {
                        minDelta = delta;
                        enter_i = i;
                        enter_j = j;
                    }
                }
            }
        }
        // Если все дельты неотрицательные, найдено оптимальное решение
        if (minDelta >= 0)
            break;

        // 3. Находим цикл (замкнутый контур) с началом в (enter_i, enter_j)
        std::vector<std::pair<int, int> > cycle = findCycle(enter_i, enter_j, isBasic);
        cycle.pop_back();
        if (cycle.empty()) {
            std::cout << "Цикл не найден! Возможно, решение вырожденно." << std::endl;
            break;
        }

        // 4. Определяем θ – минимальное значение среди базисных ячеек с отрицательным знаком
        // (в цикле чередование: +, -, +, -,...; первая (входящая) ячейка – с плюсом)
        std::int64_t theta = std::numeric_limits<std::int64_t>::max();
        int thetaIndex = -1;
        for (int k = 1; k < cycle.size(); k += 2) {
            int i = cycle[k].first;
            int j = cycle[k].second;
            if (basicPlan[i][j] < theta) {
                theta = basicPlan[i][j];
                thetaIndex = k;
            }
        }
        // 5. Обновляем базисное решение вдоль найденного цикла:
        // прибавляем θ к ячейкам с положительным знаком (чётные позиции)
        // и вычитаем θ из ячеек с отрицательным знаком (нечётные позиции)
        for (int k = 0; k < cycle.size(); k++) {
            int i = cycle[k].first;
            int j = cycle[k].second;
            if (k % 2 == 0) {
                basicPlan[i][j] += theta;
            }
            else {
                basicPlan[i][j] -= theta;
            }
        }

        std::cout << std::endl;

        // Если в ячейке с отрицательным знаком значение стало 0, то она выходит из базиса
        int remove_i = cycle[thetaIndex].first;
        int remove_j = cycle[thetaIndex].second;
        if (std::abs(basicPlan[remove_i][remove_j]) < 1e-9)
            isBasic[remove_i][remove_j] = false;

        // Входящая ячейка становится базисной
        isBasic[enter_i][enter_j] = true;
    }

    // Вывод оптимальной стоимости (можно также вернуть оптимальный план)
    double totalCost = 0;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (isBasic[i][j])
                totalCost += basicPlan[i][j] * pathCosts[i][j];
    std::cout << "Найдено оптимальное решение, стоимость = " << totalCost << std::endl;
    return basicPlan;
}

void TransportTask::computePotentials(const std::vector<std::vector<bool> > &isBasic, std::vector<double> &u,
                                      std::vector<double> &v) const {
    int m = pathCosts.size();
    int n = pathCosts[0].size();
    u.assign(m, std::numeric_limits<int>::max());
    v.assign(n, std::numeric_limits<int>::max());
    u[0] = 0; // произвольное задание одного потенциала
    bool updated = true;
    while (updated) {
        updated = false;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (isBasic[i][j]) {
                    if (u[i] != std::numeric_limits<int>::max() && v[j] == std::numeric_limits<int>::max()) {
                        v[j] = pathCosts[i][j] - u[i];
                        updated = true;
                    } else if (v[j] != std::numeric_limits<int>::max() && u[i] == std::numeric_limits<int>::max()) {
                        u[i] = pathCosts[i][j] - v[j];
                        updated = true;
                    }
                }
            }
        }
    }
}


/// Рекурсивный DFS для поиска цикла в базисных ячейках с чередованием направлений.
/// Параметр horizontal указывает, ищем ли перемещение по строке (horizontal = true) или по столбцу.
bool TransportTask::dfsCycle(int cur_i, int cur_j, int start_i, int start_j,
                             std::vector<std::vector<bool> > const &isBasic,
                             std::vector<std::pair<int, int> > &cycle,
                             std::vector<std::vector<bool> > &visited,
                             bool horizontal) {
    int m = isBasic.size();
    int n = isBasic[0].size();
    // Если вернулись в стартовую ячейку и цикл содержит хотя бы 4 элемента, цикл найден.
    if (cur_i == start_i && cur_j == start_j && cycle.size() >= 4) {
        return true;
    }
    if (horizontal) {
        // Перемещение по строке: перебираем все столбцы текущей строки.
        for (int j = 0; j < n; j++) {
            if (j == cur_j) continue;
            // Рассматриваем ячейку, если она базисная или является стартовой (для замыкания цикла)
            if (isBasic[cur_i][j] || (cur_i == start_i && j == start_j)) {
                if(visited[cur_i][j])
                    continue;
                visited[cur_i][j] = true;
                cycle.push_back({cur_i, j});
                if(dfsCycle(cur_i, j, start_i, start_j, isBasic, cycle, visited, !horizontal))
                    return true;
                cycle.pop_back();
                visited[cur_i][j] = false;
            }
        }
    } else {
        // Перемещение по столбцу: перебираем все строки текущего столбца.
        for (int i = 0; i < m; i++) {
            if (i == cur_i) continue;
            if (isBasic[i][cur_j] || (i == start_i && cur_j == start_j)) {
                if(visited[i][cur_j])
                    continue;
                visited[i][cur_j] = true;
                cycle.push_back({i, cur_j});
                if(dfsCycle(i, cur_j, start_i, start_j, isBasic, cycle, visited, !horizontal))
                    return true;
                cycle.pop_back();
                visited[i][cur_j] = false;
            }
        }
    }
    return false;
}

/// Функция, ищущая цикл (замкнутый контур) с началом в (start_i, start_j)
std::vector<std::pair<int, int> > TransportTask::findCycle(int start_i, int start_j,
                                                           std::vector<std::vector<bool> > const &isBasic) {
    int m = isBasic.size();
    int n = isBasic[0].size();
    std::vector<std::vector<bool> > visited(m, std::vector<bool>(n, false));
    std::vector<std::pair<int, int> > cycle;
    cycle.push_back({start_i, start_j});
    // Пробуем начать с горизонтального перемещения
    if (dfsCycle(start_i, start_j, start_i, start_j, isBasic, cycle, visited, true))
        return cycle;
    // Если не найдено, пробуем вертикальное перемещение
    if (dfsCycle(start_i, start_j, start_i, start_j, isBasic, cycle, visited, false))
        return cycle;
    return std::vector<std::pair<int, int> >(); // если цикл не найден
}
