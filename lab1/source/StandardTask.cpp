#include "StandardTask.h"

#include <iostream>

#include "utility/print_matrix.h"

StandardTask::StandardTask(Task const &task) {
    targetFunction = task.getTargetFunction();
    if (task.getType() == Task::Minimization) {
        for (double& coef : targetFunction) {
            coef = -coef;
        }
        targetFunctionSignChanged = true;
    }
    freeTerm = 0.0;
    n = targetFunction.size();

    for (auto &restriction: task.getRestrictions()) {
        auto& coefs = restriction.coefficients;

        switch (restriction.type) {
            case Task::Restriction::Equation:
                A.emplace_back(coefs);
                b.emplace_back(coefs.back());
                A.back().pop_back();

                A.emplace_back(coefs.size() - 1);
                for (std::size_t i = 0; i < A.back().size(); ++i) {
                    A.back()[i] = -coefs[i];
                }
                b.emplace_back(-coefs.back());
                break;
            case Task::Restriction::LessInEquation:
                A.emplace_back(coefs);
                b.emplace_back(coefs.back());
                A.back().pop_back();
                break;
            case Task::Restriction::GreaterInEquation:
                A.emplace_back(coefs.size() - 1);
                for (std::size_t i = 0; i < A.back().size(); ++i) {
                    A.back()[i] = -coefs[i];
                }
                b.emplace_back(-coefs.back());
                break;
        }
    }

    std::set<std::size_t> notRestrictedIndices;
    for (std::size_t i = 0; i < n; ++i) {
        notRestrictedIndices.insert(i);
    }

    // Смена знака у <= 0 ограничений
    for (auto &index: task.getLessThanZeroRestrictionIndices()) {
        targetFunction[index] = -targetFunction[index];
        for (auto &row: A) {
            row[index] = -row[index];
        }
        changedSignIndices.insert(index);
        notRestrictedIndices.erase(index);
    }

    for (auto &index: task.getGreaterThanZeroRestrictionIndices()) {
        notRestrictedIndices.erase(index);
    }

    // Разбиение переменных
    for (auto &index: notRestrictedIndices) {
        targetFunction.emplace_back(-targetFunction[index]);
        splitVarIndices.emplace(index, targetFunction.size() - 1);
        for (std::size_t i = 0; i < A.size(); ++i) {
            auto &row = A[i];
            row.emplace_back(-row[index]);
        }
    }
}

void StandardTask::print() {
    std::cout << "\nЗадача ЛП в стандартной форме" << std::endl;
    printTargetFunction(targetFunction);
    for (std::size_t i = 0; i < A.size(); ++i) {
        std::cout << "\t";
        for (double & coef : A[i]) {
            std::cout << std::format("{}\t", coef);
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
