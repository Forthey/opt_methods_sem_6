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
    enum savedValue {
        Left,
        Right,
        None
    } saved = None;
    double f_x1 = 0.0, f_x2 = 0.0;
    double prev_x = 0.0;

    while (b - a >= epsilon) {
        double const x1 = b - (b - a) / GOLDEN_RATIO, x2 = a + (b - a) / GOLDEN_RATIO;

        switch (saved) {
            case Left:
                if (prev_x == x1) {
                    f_x2 = f(x2);
                } else {
                    f_x2 = f_x1;
                    f_x1 = f(x1);
                }
                break;
            case Right:
                if (prev_x == x2) {
                    f_x1 = f(x1);
                } else {
                    f_x1 = f_x2;
                    f_x2 = f(x2);
                }
                break;
            case None:
                f_x1 = f(x1);
                f_x2 = f(x2);
                break;
        }

        if (f_x1 < f_x2) {
            saved = Left;
            prev_x = x1;
            b = x2;
        } else if (f_x2 < f_x1) {
            saved = Right;
            prev_x = x2;
            a = x1;
        } else {
            saved = None;
            a = x1;
            b = x2;
        }
    }

    return (a + b) / 2.0;
}
