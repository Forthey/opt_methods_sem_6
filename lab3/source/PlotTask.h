#pragma once
#include "alg/FunctionWrapper.h"


class PlotTask {
    static double TRUE_RESULT;
public:
    static void doTask(FunctionWrapper& f, int minDegree, int maxDegree, std::size_t funcPoints);
};

