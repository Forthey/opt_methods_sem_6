#include "print_matrix.h"

#include <iostream>

void printTargetFunction(Vector<double> const &targetFunction) {
    std::cout << "z: \t";
    for (std::size_t i = 0; i < targetFunction.size() - 1; ++i) {
        std::cout << std::format("{}\t", targetFunction[i]);
    }
    std::cout << targetFunction.back() << std::endl;
}

void printConditions(Matrix<double> const &equations, std::string const &rowConditionType) {
    for (auto& row : equations) {
        std::cout << "\t";
        for (std::size_t i = 0; i < row.size(); ++i) {
            std::cout << std::format("{}\t", row[i]);
        }
        std::cout << std::format("\"{}\"\n", rowConditionType);
    }
}
