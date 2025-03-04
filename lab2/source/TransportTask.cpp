#include "TransportTask.h"

#include <fstream>
#include <iostream>

#include "utility/incorrect_format_exception.h"


void TransportTask::balanceTask() {
    std::size_t supply = 0, demand = 0;
    for (std::size_t supplier : suppliers) {
        supply += supplier;
    }
    for (std::size_t consumer : consumers) {
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

    std::size_t suppliersCnt, consumersCnt;

    if (!(file >> suppliersCnt >> consumersCnt)) {
        throw IncorrectFormatException(filename);
    }

    suppliers.resize(suppliersCnt);
    consumers.resize(consumersCnt);
    pathCosts.resize(suppliersCnt);

    for (std::size_t i = 0; i < suppliersCnt; ++i) {
        if (!(file >> suppliers[i])) {
            throw IncorrectFormatException(filename);
        }
    }

    for (std::size_t i = 0; i < consumersCnt; ++i) {
        if (!(file >> consumers[i])) {
            throw IncorrectFormatException(filename);
        }
    }

    for (std::size_t i = 0; i < suppliersCnt; ++i) {
        pathCosts[i].resize(consumersCnt);
        for (std::size_t j = 0; j < consumersCnt; ++j) {
            if (!(file >> pathCosts[i][j])) {
                throw IncorrectFormatException(filename);
            }
        }
    }

    balanceTask();
}

std::vector<std::vector<std::size_t>> TransportTask::northwestCornerMethod() {
    std::vector basicPlan(pathCosts.size(), std::vector<std::size_t>(pathCosts[0].size()));
    std::vector suppliers = this->suppliers, consumers = this->consumers;

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
