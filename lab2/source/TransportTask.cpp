#include "TransportTask.h"

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "utility/incorrect_format_exception.h"


TransportTask::TransportTask(std::string const &filename) {
    std::ifstream file(std::format("tasks/{}.json", filename));

    if (!file.is_open()) {
        std::cerr << "Невозможно отрыть файл: " << filename << std::endl;
        exit(1);
    }

    std::string data;
    std::stringstream ss;
    ss << file.rdbuf();
    data = ss.str();

    parseFileData(data);


    balanceTask();
}


void TransportTask::parseFileData(std::string const &data) {
    nlohmann::json json;
    try {
        json = nlohmann::json::parse(data);

        suppliers = Row<double>(json.at("suppliers"));
        consumers = Row<double>(json.at("consumers"));
        pathCosts = Table<double>(json.at("path_costs"));

        if (json.contains("supplier_penalty")) {
            penalty = Row<double>(json.at("supplier_penalty"));
        }
        if (json.contains("consumer_penalty")) {
            penalty = Row<double>(json.at("consumer_penalty"));
        }
    } catch (nlohmann::json::parse_error const &) {
    } catch (nlohmann::json::type_error const &) {
        throw IncorrectFormatException();
    }
}

void TransportTask::balanceTask() {
    double supply = 0.0, demand = 0.0;
    for (double const supplier: suppliers) {
        supply += supplier;
    }
    for (double const consumer: consumers) {
        demand += consumer;
    }
    if (demand > supply) {
        fakeOne = Supplier;
        suppliers.emplace_back(demand - supply);
        pathCosts.emplace_back(penalty);
    } else if (demand < supply) {
        fakeOne = Consumer;
        consumers.emplace_back(supply - demand);
        for (std::size_t i = 0; i < suppliers.size(); i++) {
            pathCosts[i].emplace_back(penalty[i]);
        }
    }
}


Table<double> TransportTask::northwestCornerMethod() {
    Table basicPlan(pathCosts.size(), Row<double>(pathCosts[0].size()));
    Row suppliers = this->suppliers, consumers = this->consumers;

    std::size_t supIndex = 0, consIndex = 0;

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


Table<double> TransportTask::potentialsMethod(
    Table<double> &basicPlan) const {
    std::size_t const m = suppliers.size(), n = consumers.size();
    Row<double> u(m), v(n);

    Table isBasic(m, Row(n, false));

    // Помечаем базисные клетки
    for (std::size_t i = 0; i < m; ++i) {
        for (std::size_t j = 0; j < n; ++j) {
            if (basicPlan[i][j] > 0) {
                isBasic[i][j] = true;
            }
        }
    }

    while (true) {
        // Вычисляем потенциалы
        computePotentials(isBasic, u, v);

        // Проверяем условие оптимальности: для всех не-базисных ячеек вычисляем дельту
        double minDelta = 0;
        std::size_t enter_i = 0, enter_j = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (!isBasic[i][j]) {
                    // не-базисная ячейка
                    double delta = pathCosts[i][j] - (u[i] + v[j]);
                    if (delta < minDelta) {
                        minDelta = delta;
                        enter_i = i;
                        enter_j = j;
                    }
                }
            }
        }
        // Если все дельты неотрицательные, найдено оптимальное решение
        if (minDelta >= 0.0)
            break;

        // Находим цикл (замкнутый контур) с началом в (enter_i, enter_j)
        std::vector<std::pair<std::size_t, std::size_t>> cycle = findCycle(enter_i, enter_j, isBasic);
        cycle.pop_back();
        if (cycle.empty()) {
            std::cerr << "Цикл не найден! Возможно, решение вырожденное" << std::endl;
            return {};
        }

        // Определяем θ – минимальное значение среди базисных ячеек с отрицательным знаком
        // (в цикле чередование: +, -, +, -, ...; первая (входящая) ячейка – с плюсом)
        double theta = std::numeric_limits<double>::max();
        std::size_t thetaIndex = -1;
        for (std::size_t k = 1; k < cycle.size(); k += 2) {
            auto& [i, j] = cycle[k];
            if (basicPlan[i][j] < theta) {
                theta = basicPlan[i][j];
                thetaIndex = k;
            }
        }

        // Обновляем базисное решение вдоль найденного цикла:
        // прибавляем θ к ячейкам с положительным знаком (чётные позиции)
        // и вычитаем θ из ячеек с отрицательным знаком (нечётные позиции)
        for (std::size_t k = 0; k < cycle.size(); k++) {
            auto& [i, j] = cycle[k];
            if (k % 2 == 0) {
                basicPlan[i][j] += theta;
            }
            else {
                basicPlan[i][j] -= theta;
            }
        }

        // Если в ячейке с отрицательным знаком значение стало 0, то она выходит из базиса
        auto& [removeI, removeJ] = cycle[thetaIndex];
        if (std::abs(basicPlan[removeI][removeJ]) < 1e-9)
            isBasic[removeI][removeJ] = false;

        // Входящая ячейка становится базисной
        isBasic[enter_i][enter_j] = true;
    }

    return basicPlan;
}


void TransportTask::computePotentials(const std::vector<std::vector<bool> > &isBasic, std::vector<double> &u,
                                      std::vector<double> &v) const {
    std::size_t const m = suppliers.size(), n = consumers.size();

    u.assign(m, std::numeric_limits<double>::max());
    v.assign(n, std::numeric_limits<double>::max());
    u[0] = 0; // произвольное задание одного потенциала

    bool updated = true;
    while (updated) {
        updated = false;
        for (std::size_t i = 0; i < m; i++) {
            for (std::size_t j = 0; j < n; j++) {
                if (isBasic[i][j]) {
                    if (u[i] != std::numeric_limits<double>::max() && v[j] == std::numeric_limits<double>::max()) {
                        v[j] = pathCosts[i][j] - u[i];
                        updated = true;
                    } else if (v[j] != std::numeric_limits<double>::max() && u[i] == std::numeric_limits<double>::max()) {
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
bool TransportTask::dfsCycle(std::size_t cur_i, std::size_t cur_j, std::size_t start_i, std::size_t start_j,
                             std::vector<std::vector<bool> > const &isBasic,
                             std::vector<std::pair<std::size_t, std::size_t> > &cycle,
                             std::vector<std::vector<bool> > &visited,
                             bool horizontal) {
    std::size_t const m = isBasic.size(), n = isBasic[0].size();

    // Если вернулись в стартовую ячейку и цикл содержит хотя бы 4 элемента, цикл найден.
    if (cur_i == start_i && cur_j == start_j && cycle.size() >= 4) {
        return true;
    }
    if (horizontal) {
        // Перемещение по строке: перебираем все столбцы текущей строки.
        for (std::size_t j = 0; j < n; j++) {
            if (j == cur_j) continue;
            // Рассматриваем ячейку, если она базисная или является стартовой (для замыкания цикла)
            if (isBasic[cur_i][j] || (cur_i == start_i && j == start_j)) {
                if (visited[cur_i][j])
                    continue;
                visited[cur_i][j] = true;
                cycle.emplace_back(cur_i, j);
                if(dfsCycle(cur_i, j, start_i, start_j, isBasic, cycle, visited, false))
                    return true;
                cycle.pop_back();
                visited[cur_i][j] = false;
            }
        }
    } else {
        // Перемещение по столбцу: перебираем все строки текущего столбца.
        for (std::size_t i = 0; i < m; i++) {
            if (i == cur_i) continue;
            if (isBasic[i][cur_j] || (i == start_i && cur_j == start_j)) {
                if(visited[i][cur_j])
                    continue;
                visited[i][cur_j] = true;
                cycle.emplace_back(i, cur_j);
                if(dfsCycle(i, cur_j, start_i, start_j, isBasic, cycle, visited, true))
                    return true;
                cycle.pop_back();
                visited[i][cur_j] = false;
            }
        }
    }
    return false;
}


/// Функция, ищущая цикл (замкнутый контур) с началом в (start_i, start_j)
std::vector<std::pair<std::size_t, std::size_t>> TransportTask::findCycle(std::size_t start_i, std::size_t start_j,
                                                           std::vector<std::vector<bool> > const &isBasic) {
    std::size_t m = isBasic.size(), n = isBasic[0].size();

    Table visited(m, Row(n, false));
    std::vector<std::pair<std::size_t, std::size_t>> cycle;
    cycle.emplace_back(start_i, start_j);
    // Пробуем начать с горизонтального перемещения
    if (dfsCycle(start_i, start_j, start_i, start_j, isBasic, cycle, visited, true))
        return cycle;
    // Если не найдено, пробуем вертикальное перемещение
    if (dfsCycle(start_i, start_j, start_i, start_j, isBasic, cycle, visited, false))
        return cycle;
    return {}; // если цикл не найден
}

double TransportTask::calculateCosts(Table<double> const &plan) const {
    double totalCost = 0.0;
    for (std::size_t i = 0; i < suppliers.size(); i++) {
        for (std::size_t j = 0; j < consumers.size(); j++) {
            totalCost += plan[i][j] * pathCosts[i][j];
        }
    }

    return totalCost;
}

Table<double> TransportTask::restorePlan(Table<double> const &plan) const {
    Table<double> restoredPlan = plan;
    switch (fakeOne) {
        case Noone:
            break;
        case Consumer:
            for (Row<double>& row : restoredPlan) {
                row.pop_back();
            }
            break;
        case Supplier:
            restoredPlan.pop_back();
            break;
    }
    return restoredPlan;
}
