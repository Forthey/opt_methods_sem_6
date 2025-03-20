#include "AlgsForExtremes.h"

#include <cmath>


constexpr double GOLDEN_RATIO = (1.0 + std::sqrt(5.0)) / 2.0;


double AlgsForExtremes::findMinViaHalfDivisionMethod(FunctionWrapper &f, double a, double b, double epsilon) {
    double const delta = epsilon / 4.0;

    while (b - a >= epsilon) {
        double const
                x1 = (a + b) / 2.0 - delta, x2 = (a + b) / 2.0 + delta,
                f_x1 = f(x1), f_x2 = f(x2);

        if (f_x1 < f_x2) {
            b = x2;
        } else if (f_x2 < f_x1) {
            a = x1;
        } else {
            a = x1;
            b = x2;
        }
    }

    return (a + b) / 2.0;
}

double AlgsForExtremes::findMinViaGoldenRatioMethod(FunctionWrapper &f, double a, double b, double epsilon) {
    while (b - a >= epsilon) {
        double const
                x1 = b - (b - a) / GOLDEN_RATIO, x2 = a + (b - a) / GOLDEN_RATIO,
                f_x1 = f(x1), f_x2 = f(x2);

        if (f_x1 < f_x2) {
            b = x2;
        } else if (f_x2 < f_x1) {
            a = x1;
        } else {
            a = x1;
            b = x2;
        }
    }

    return (a + b) / 2.0;
}
