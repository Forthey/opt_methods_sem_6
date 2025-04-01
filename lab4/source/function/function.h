#pragma once
#include "RealVector.h"


inline double function(RealVector<2> const& x) {
    return x[0] * x[0] * x[0] * x[0] + x[1] * x[1] * x[1] * x[1] + std::sin(x[0]) + std::sin(x[1]);
}

inline double functionDerivative(RealVector<2> const& x) {
    return 4 * x[0] * x[0] * x[0] + 4 * x[1] * x[1] * x[1] + std::cos(x[0]) + std::cos(x[1]);
}

inline double functionDerivative2(RealVector<2> const& x) {
    return 12 * x[0] * x[0] + 12 * x[1] * x[1] - std::sin(x[0]) - std::sin(x[1]);
}