#include <cmath>
#include <iostream>

#include "FunctionWrapper.h"


double f(double const x) {
    return 0.5 * x * x - std::sin(x);
}

bool fValidator(double const x) {
    static double max = std::sqrt(std::numeric_limits<double>::max());

    return x < max;
}

int main() {
    FunctionWrapper wrapper(f, fValidator);

    std::cout << wrapper(0.5) << std::endl;
    std::cout << wrapper(std::numeric_limits<double>::max() / 100.0);
    return 0;
}