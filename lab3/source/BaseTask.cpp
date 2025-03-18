#include "BaseTask.h"

#include <iostream>
#include <sstream>

#include "alg/AlgsForExtremes.h"


std::string BaseTask::toStringWithPrecision(double const value, double epsilon) {
    int precision = 0;
    while (epsilon < 1.0) {
        epsilon *= 10.0;
        precision++;
    }

    std::ostringstream out;
    out.precision(precision);
    out << std::fixed << value;

    return std::move(out).str();
}


void BaseTask::doTask(FunctionWrapper &f) {
    auto epsilons = {0.1, 0.01, 0.001};

    for (double epsilon: epsilons) {
        double result = AlgsForExtremes::findMinViaHalfDivisionMethod(f, 0.0, 1.0, epsilon);
        std::size_t useCount = f.getUseCount();
        std::cout << std::format(
            "Результат методом половинного деления:\t e = {} \tx = {} \t{} вызовов функции f(x)\n",
            epsilon, toStringWithPrecision(result, epsilon), useCount
        );
        f.resetUseCount();

        result = AlgsForExtremes::findMinViaGoldenRatioMethod(f, 0.0, 1.0, epsilon);
        useCount = f.getUseCount();
        std::cout << std::format(
            "Результат методом золотого сечения:\t e = {} \tx = {} \t{} вызовов функции f(x)\n",
            epsilon, toStringWithPrecision(result, epsilon), useCount
        );
        f.resetUseCount();
    }
}
