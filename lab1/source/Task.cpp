#include "Task.h"

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "utility/print_matrix.h"


Task::Task(std::string const &filename) {
    type = TaskType::Maximization;

    std::ifstream file(std::format("tasks/{}.json", filename));
    if (!file.is_open()) {
        std::cerr << "Невозможно открыть файл " << filename << std::endl;
        exit(1);
    }

    try {
        nlohmann::json json = nlohmann::json::parse(file);

        targetFunction = json["target_function"].get<Vector<double> >();

        auto equations = json["equations"].get<Matrix<double> >();
        for (auto const &equation: equations) {
            restrictions.emplace_back(Restriction::Type::Equation, equation);
        }
        auto lessInEquations = json["less_in_equations"].get<Matrix<double> >();
        for (auto const &lessInEquation: lessInEquations) {
            restrictions.emplace_back(Restriction::Type::LessInEquation, lessInEquation);
        }
        auto greaterInEquations = json["greater_in_equations"].get<Matrix<double> >();
        for (auto const &greaterInEquation: greaterInEquations) {
            restrictions.emplace_back(Restriction::Type::GreaterInEquation, greaterInEquation);
        }

        greaterThanZeroRestrictionIndices = json["greater_than_zero_restriction_indices"].get<Vector<std::size_t> >();
        for (auto &index: greaterThanZeroRestrictionIndices) {
            --index;
        }
        lessThanZeroRestrictionIndices = json["less_than_zero_restriction_indices"].get<Vector<std::size_t> >();
        for (auto &index: lessThanZeroRestrictionIndices) {
            --index;
        }
    } catch (nlohmann::json::parse_error const &) {
        std::cerr << "Неверный формат json файла" << std::endl;
        exit(1);
    }
}

Task *Task::makeDualTask() const {
    auto task = new Task();

    task->type = type == Maximization ? Minimization : Maximization;

    for (std::size_t i = 0; i < restrictions.size(); ++i) {
        auto& restriction = restrictions[i];

        task->targetFunction.emplace_back(restriction.coefficients.back());
        switch (restriction.type) {
            case Restriction::Equation:
                break;
            case Restriction::LessInEquation:
                task->greaterThanZeroRestrictionIndices.emplace_back(i);
                break;
            case Restriction::GreaterInEquation:
                task->lessThanZeroRestrictionIndices.emplace_back(i);
                break;
        }
    }

    for (std::size_t i = 0; i < targetFunction.size(); ++i) {
        Restriction::Type type;
        if (std::ranges::find(lessThanZeroRestrictionIndices, i) != lessThanZeroRestrictionIndices.end()) {
            type = Restriction::Type::LessInEquation;
        } else if (std::ranges::find(greaterThanZeroRestrictionIndices, i) != greaterThanZeroRestrictionIndices.end()) {
            type = Restriction::Type::GreaterInEquation;
        } else {
            type = Restriction::Type::Equation;
        }

        task->restrictions.emplace_back(type, Vector<double>(restrictions.size()));

        for (std::size_t j = 0; j < restrictions.size(); ++j) {
            task->restrictions[i].coefficients[j] = restrictions[j].coefficients[i];
        }

        task->restrictions[i].coefficients.emplace_back(targetFunction[i]);
    }

    return task;
}

void Task::print() {
    std::cout << "\nЗадача ЛП" << std::endl;
    printTargetFunction(targetFunction);

    for (auto &row: restrictions) {
        std::cout << "\t";
        for (std::size_t i = 0; i < row.coefficients.size(); ++i) {
            std::cout << std::format("{}\t", row.coefficients[i]);
        }

        switch (row.type) {
            case Restriction::Equation:
                std::cout << "\"=\"\n";
                break;
            case Restriction::LessInEquation:
                std::cout << "\"<=\"\n";
                break;
            case Restriction::GreaterInEquation:
                std::cout << "\">=\"\n";
                break;
        }
    }

    if (!greaterThanZeroRestrictionIndices.empty()) {
        for (std::size_t i = 0; i < greaterThanZeroRestrictionIndices.size(); ++i) {
            std::cout << std::format("x{}{}", greaterThanZeroRestrictionIndices[i] + 1,
                                     i == greaterThanZeroRestrictionIndices.size() - 1 ? " >= 0\n" : ", ");
        }
    }
    if (!lessThanZeroRestrictionIndices.empty()) {
        for (std::size_t i = 0; i < lessThanZeroRestrictionIndices.size(); ++i) {
            std::cout << std::format("x{}{}", lessThanZeroRestrictionIndices[i] + 1,
                                     i == lessThanZeroRestrictionIndices.size() - 1 ? " <= 0\n" : ", ");
        }
    }
}
