#pragma once
#include "FunctionWrapper.h"


class AlgsForExtremes {
public:
    [[nodiscard]] static double findMinViaHalfDivisionMethod(FunctionWrapper& f, double a, double b, double epsilon);

    [[nodiscard]] static double findMinViaGoldenRatioMethod(FunctionWrapper& f, double a, double b, double epsilon);
};
