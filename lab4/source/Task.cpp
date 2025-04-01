#include "Task.h"

#include <iostream>
#include <cmath>

#include "alg/AlgsForExtremes.h"


RealVector<2> const Task::trueMin(-0.1571849514838140095869566960, -1.157184951483814009586956696);


double Task::function(RealVector<2> const &x) {
    return std::exp(x[0] - 1.0) + x[0] * x[0] + std::exp(x[1]) + (x[1] + 1.0) * (x[1] + 1.0);
}

RealVector<2> Task::functionGradient(RealVector<2> const &x) {
    return RealVector(std::exp(x[0] - 1.0) + 2.0 * x[0], std::exp(x[1]) + 2 * (x[1] + 1.0));
}

RealMatrix<2> Task::functionInvertedHessian(RealVector<2> const &x) {
    double
        a = std::exp(x[0] - 1.0) + 2.0,
        b = 0.0,
        c = 0.0,
        d = std::exp(x[1]) + 2.0;
    double coef = 1 / (a * d - b * c);
    return RealMatrix(
        RealVector(d, -b) * coef,
        RealVector(-c, a) * coef
    );
}

void Task::gradientMethodCalcs() {
    auto epsilons = {"0.1", "0.01", "0.001"};
    FunctionWrapper<2, double> f(function);
    FunctionWrapper<2, RealVector<2>> df(functionGradient);
    FunctionWrapper<2, RealMatrix<2>> hf(functionInvertedHessian);

    std::cout << "GRADIENT METHOD" << std::endl;

    for (auto& epsilon : epsilons) {
        std::cout << std::format(
            "e = {} \t{}\n",
            epsilon,
            AlgsForExtremes::gradientMethod<2>(f, df, 0.1).toString(std::atof(epsilon))
        );
    }
}

void Task::newtonMethodCalcs() {
    auto epsilons = {"0.1", "0.01", "0.001"};
    FunctionWrapper<2, double> f(function);
    FunctionWrapper<2, RealVector<2>> df(functionGradient);
    FunctionWrapper<2, RealMatrix<2>> hf(functionInvertedHessian);

    std::cout << "NEWTON METHOD" << std::endl;

    for (auto& epsilon : epsilons) {
        std::cout << std::format(
            "e = {} \t{}\n",
            epsilon,
            AlgsForExtremes::newtonMethod<2>(f, df, hf, 0.1).toString(std::atof(epsilon))
        );
    }
}
