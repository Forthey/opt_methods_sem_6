#pragma once

#include <string>

#include "utility/custom_types.h"

class Task {
    Vector<double> targetFunction;
    Matrix<double> equations;
    Matrix<double> lessInEquations;
    Matrix<double> greaterInEquations;
    Vector<std::size_t> greaterThanZeroRestrictionIndices;
    Vector<std::size_t> lessThanZeroRestrictionIndices;

public:
    explicit Task(std::string const &filename);

    Vector<double> const &getTargetFunction() const { return targetFunction; }

    Matrix<double> const &getEquations() const { return equations; }

    Matrix<double> const &getLessInEquations() const { return lessInEquations; }

    Matrix<double> const &getGreaterInEquations() const { return greaterInEquations; }

    Vector<std::size_t> const &getGreaterThanZeroRestrictionIndices() const { return greaterThanZeroRestrictionIndices; }

    Vector<std::size_t> const &getLessThanZeroRestrictionIndices() const { return lessThanZeroRestrictionIndices; }

    void print();
};
