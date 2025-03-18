#include <cmath>
#include <iostream>

#include "alg/AlgsForExtremes.h"


int main() {
    FunctionWrapper wrapper(
        [](double x) -> double { return 0.5 * x * x - std::sin(x); },
        [](double x) -> bool { return 0.0 <= x && x <= 1.0; }
    );

    std::cout << AlgsForExtremes::findMinViaHalfDivisionMethod(wrapper, 0.0, 1.0, 0.0001) << std::endl;

    std::cout << AlgsForExtremes::findMinViaGoldenRatioMethod(wrapper, 0.0, 1.0, 0.0001) << std::endl;
    return 0;
}