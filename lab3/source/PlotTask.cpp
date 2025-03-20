#include "PlotTask.h"

#include <cmath>
#include <fstream>
#include <iostream>

#include "alg/AlgsForExtremes.h"


double PlotTask::TRUE_RESULT = 0.7390851332151606416553120876738734040134;


void PlotTask::doTask(FunctionWrapper &f, int minDegree, int maxDegree, std::size_t funcPoints) {
    std::ofstream csvFile("results/data.csv");

    if (!csvFile.is_open()) {
        std::cerr << "Ошибка при открытии файла results/data.csv. Проверьте права на запись" << std::endl;
        return;
    }

    minDegree = std::clamp(minDegree, -14, 1);
    maxDegree = std::clamp(maxDegree, minDegree, 1);

    csvFile << "e, МПД, |МПД - min|, N(МПД), МЗС, |МЗС - min|, N(МЗС)\n";
    for (int degree = minDegree; degree <= maxDegree; degree++) {
        double const epsilon = std::pow(10.0, degree);
        double mpdResult = AlgsForExtremes::findMinViaHalfDivisionMethod(f, 0.0, 1.0, epsilon);
        std::size_t mpdUseCount = f.getUseCount();
        f.resetUseCount();

        double mzsResult = AlgsForExtremes::findMinViaGoldenRatioMethod(f, 0.0, 1.0, epsilon);
        std::size_t mzsUseCount = f.getUseCount();
        f.resetUseCount();

        csvFile << std::format(
            "{}, {}, {}, {}, {}, {}, {}\n",
            epsilon,
            mpdResult, std::abs(mpdResult - TRUE_RESULT), mpdUseCount,
            mzsResult, std::abs(mzsResult - TRUE_RESULT), mzsUseCount
        );
    }

    csvFile.close();
    csvFile.open("results/func_values.csv");

    if (!csvFile.is_open()) {
        std::cerr << "Ошибка при открытии файла results/func_values.csv. Проверьте права на запись" << std::endl;
        return;
    }

    funcPoints = std::max(2ull, funcPoints);

    csvFile << "x, f(x)\n";

    double x = 0.0;
    double const step = (1.0 - 0.0) / (funcPoints - 1);
    for (std::size_t i = 0; i < funcPoints; ++i) {
        csvFile << std::format("{}, {}\n", x, f(x));
        x += step;
    }
}
