#include "Task.h"

#include <iostream>
#include <cmath>

#include "alg/AlgsForExtremes.h"



static double functionDerivative2(RealVector<2> const &x) {
    return 12.0 * x[0] * x[0] + 12 * x[1] * x[1] - std::sin(x[0]) - std::sin(x[1]);
}

RealVector<2> const Task::trueMin(-0.4175620572302547997715429285, - -0.4501836112948735730365386968);


double Task::function(RealVector<2> const &x) {
    return x[0] * x[0] + x[1] * x[1] + std::sin(x[0] + 1.0) + std::sin(x[1]);
    // return (x[0] - 1.0) * (x[0] - 1.0) * (x[0] - 1.0) * (x[0] - 1.0) + x[1] * x[1] * x[1] * x[1];
}

RealVector<2> Task::functionGradient(RealVector<2> const &x) {
    return RealVector(2 * x[0] + std::cos(x[0] + 1.0), 2 * x[1] + std::cos(x[1]));
    // return RealVector(4 * (x[0] - 1.0) * (x[0] - 1.0) * (x[0] - 1.0), 4 * x[1] * x[1] * x[1]);
}

void Task::gradientMethodCalcs() {
    auto epsilons = {"0.1", "0.01", "0.0001"};
    FunctionWrapper<2, double> func(function);
    FunctionWrapper<2, RealVector<2>> dfunc(functionGradient);

    for (auto& epsilon : epsilons) {
        std::cout << std::format(
            "e = {} \t{}\n",
            epsilon,
            AlgsForExtremes::gradientMethod<2>(func, dfunc, 0.1).toString(std::atof(epsilon))
        );
    }
}
