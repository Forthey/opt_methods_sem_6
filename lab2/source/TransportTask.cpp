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

void TransportTask::northwestCornerMethod() {
}
