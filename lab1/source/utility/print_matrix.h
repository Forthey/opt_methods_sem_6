#pragma once
#include <string>

#include "custom_types.h"

void printTargetFunction(Vector<double> const& targetFunction);

void printConditions(Matrix<double> const& equations, std::string const& rowConditionType = "<=");