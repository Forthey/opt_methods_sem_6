#pragma once
#include "alg/FunctionWrapper.h"


class BaseTask {
    static std::string toStringWithPrecision(double value, double epsilon);
public:
    static void doTask(FunctionWrapper &f);
};
