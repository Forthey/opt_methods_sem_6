#pragma once
#include "function/FunctionWrapper.h"


constexpr double GOLDEN_RATIO = (1.0 + std::sqrt(5.0)) / 2.0;


class AlgsForExtremes {
public:
    AlgsForExtremes() = delete;

    template <std::uint8_t argNum>
    static RealVector<argNum> gradientMethod(FunctionWrapper<argNum> const& f, FunctionWrapper<argNum> const& df, double epsilon = 0.01) {
        RealVector<argNum> x1, x2;
        double df_x1 = 0.0;

        auto wrapper = FunctionWrapper<1>(
            [&df_x1, &x1, &f](RealVector<1> const& coef) -> double {
                return f(x1 - coef[0] * df_x1);
        });

        do {
            x1 = x2;
            df_x1 = df(x1);

            double coef = goldenRatioMethod(wrapper, 0.0, 10.0, epsilon);

            x2 = x1 - coef * df(x2);
        } while (x1.normOfDifference(x2) >= epsilon);

        return x2;
    }

    static double goldenRatioMethod(FunctionWrapper<1> const&f, double a, double b, double epsilon) {
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
};
