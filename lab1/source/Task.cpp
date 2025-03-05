#include "Task.h"

#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

#include "utility/print_matrix.h"


Task::Task(std::string const &filename) {
    std::ifstream file(std::format("tasks/{}.json", filename));
    if (!file.is_open()) {
        std::cerr << "Невозможно открыть файл " << filename << std::endl;
        exit(1);
    }

    try {
        nlohmann::json json = nlohmann::json::parse(file);

        targetFunction = json["target_function"].get<Vector<double> >();
        equations = json["equations"].get<Matrix<double> >();
        lessInEquations = json["less_in_equations"].get<Matrix<double> >();
        greaterInEquations = json["greater_in_equations"].get<Matrix<double> >();
        greaterThanZeroRestrictionIndices = json["greater_than_zero_restriction_indices"].get<Vector<std::size_t>>();
        for (auto& index : greaterThanZeroRestrictionIndices) {
            --index;
        }
        lessThanZeroRestrictionIndices = json["less_than_zero_restriction_indices"].get<Vector<std::size_t> >();
        for (auto& index : lessThanZeroRestrictionIndices) {
            --index;
        }
    } catch (nlohmann::json::parse_error const &) {
        std::cerr << "Неверный формат json файла" << std::endl;
        exit(1);
    }
}

void Task::print() {
    std::cout << "\nЗадача ЛП" << std::endl;
    printTargetFunction(targetFunction);
    printConditions(lessInEquations, "<=");
    printConditions(greaterInEquations, ">=");
    printConditions(equations, "=");

    if (!greaterThanZeroRestrictionIndices.empty()) {
        for (std::size_t i = 0; i < greaterThanZeroRestrictionIndices.size(); ++i) {
            std::cout << std::format("x{}{}", greaterThanZeroRestrictionIndices[i] + 1,
                                     i == greaterThanZeroRestrictionIndices.size() - 1 ? " >= 0\n" : ", ");
        }
    }
    if (!lessThanZeroRestrictionIndices.empty()) {
        for (std::size_t i = 0; i < lessThanZeroRestrictionIndices.size(); ++i) {
            std::cout << std::format("x{}{}", lessThanZeroRestrictionIndices[i] + 1,
                                     i == lessThanZeroRestrictionIndices.size() - 1 ? " >= 0\n" : ", ");
        }
    }
}
