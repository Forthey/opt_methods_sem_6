#include "StandardTask.h"

#include <iostream>

#include "utility/print_matrix.h"

StandardTask::StandardTask(Task const& task) {
    targetFunction = task.getTargetFunction();
    freeTerm = targetFunction.back();
    targetFunction.pop_back();
    n = targetFunction.size();

    for (auto& lessInEq : task.getLessInEquations()) {
        A.emplace_back(lessInEq);
        b.emplace_back(lessInEq.back());
        A.back().pop_back();
    }

    // Добавление >= как <= с противоположными по знаку коэффициентами
    for (auto& greaterIneq : task.getGreaterInEquations()) {
        A.emplace_back(greaterIneq.size() - 1);
        for (std::size_t i = 0; i < A.back().size(); ++i) {
            A.back()[i] = -greaterIneq[i];
        }
        b.emplace_back(-greaterIneq.back());
    }

    // Добавление = как композиции <= и >=
    for (auto& equation : task.getEquations()) {
        A.emplace_back(equation);
        b.emplace_back(equation.back());
        A.back().pop_back();

        A.emplace_back(equation.size() - 1);
        for (std::size_t i = 0; i < A.back().size(); ++i) {
            A.back()[i] = -equation[i];
        }
        b.emplace_back(-equation.back());
    }

    std::set<std::size_t> notRestrictedIndices;
    for (std::size_t i = 0; i < n; ++i) {
        notRestrictedIndices.insert(i);
    }

    // Смена знака у <= 0 ограничений
    for (auto& index : task.getLessThanZeroRestrictionIndices()) {
        targetFunction[index] = -targetFunction[index];
        for (auto& row : A) {
            row[index] = -row[index];
        }
        changedSignIndices.insert(index);
        notRestrictedIndices.erase(index);
    }

    for (auto& index : task.getGreaterThanZeroRestrictionIndices()) {
        notRestrictedIndices.erase(index);
    }

    // Разбиение переменных
    for (auto& index : notRestrictedIndices) {
        targetFunction.emplace_back(-targetFunction[index]);
        splitVarIndices.emplace(index, targetFunction.size() - 1);
        for (auto& row : A) {
            row.emplace_back(-row[index]);
        }
    }
}

void StandardTask::print() {
    std::cout << "\nЗадача ЛП в стандартной форме" << std::endl;
    printTargetFunction(targetFunction);
    for (std::size_t i = 0; i < A.size(); ++i) {
        std::cout << "\t";
        for (std::size_t j = 0; j < A[i].size(); ++j) {
            std::cout << std::format("{}\t", A[i][j]);
        }
        std::cout << b[i] << std::endl;
    }
}

std::vector<double> StandardTask::getTrueValues(std::vector<double> const &x) const {
    std::vector<double> trueX(n);
    for (std::size_t i = 0; i < n; i++) {
        if (splitVarIndices.contains(i)) {
            trueX[i] = x[i] - x[splitVarIndices.at(i)];
        } else if (changedSignIndices.contains(i)) {
            trueX[i] = -x[i];
        } else {
            trueX[i] = x[i];
        }
    }

    return trueX;
}
