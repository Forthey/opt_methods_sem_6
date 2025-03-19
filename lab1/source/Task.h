#pragma once

#include <string>

#include "utility/custom_types.h"

class Task {
public:
    enum TaskType {
        Minimization,
        Maximization,
    };

    struct Restriction {
        enum Type {
            Equation,
            LessInEquation,
            GreaterInEquation,
        } type;
        Vector<double> coefficients;
    };
private:
    TaskType type;
    Vector<double> targetFunction;
    Vector<Restriction> restrictions;
    Vector<std::size_t> greaterThanZeroRestrictionIndices;
    Vector<std::size_t> lessThanZeroRestrictionIndices;

    Task() = default;
public:

    explicit Task(std::string const &filename);

    Task* makeDualTask() const;

    TaskType getType() const { return type; }

    Vector<double> const &getTargetFunction() const { return targetFunction; }

    Vector<Restriction> const &getRestrictions() const { return restrictions; }

    Matrix<double> getRestrictionsAsMatrix() const;

    Vector<std::size_t> const &getGreaterThanZeroRestrictionIndices() const { return greaterThanZeroRestrictionIndices; }

    Vector<std::size_t> const &getLessThanZeroRestrictionIndices() const { return lessThanZeroRestrictionIndices; }

    void print();
};
