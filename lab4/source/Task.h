#pragma once
#include "function/RealMatrix.h"
#include "function/RealVector.h"


class Task {
    static RealVector<2> const trueMin;
public:
    Task() = delete;

    static double function(RealVector<2> const &x);

    static RealVector<2> functionGradient(RealVector<2> const &x);

    static RealMatrix<2> functionInvertedHessian(RealVector<2> const &x);

    static void printFunction();

    static void gradientMethodCalcs();

    static void newtonMethodCalcs();

    static void hookeJeevesCalcs();

    static void checkForOrthogonalityInGradient();

    static void gradientWithNewtonCalcs();

    static void hookeJeevesWithNewtonCalcs();
};
