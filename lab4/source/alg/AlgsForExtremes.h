#pragma once
#include <memory>

#include "function/FunctionWrapper.h"


constexpr double GOLDEN_RATIO = (1.0 + std::sqrt(5.0)) / 2.0;

RealVector const DEFAULT_X(1.0, 1.0);


class AlgsForExtremes {
    template<std::uint8_t argNum>
    static RealVector<argNum> exploratorySearch(RealVector<argNum> const &x, double step,
                                                RealVector<argNum> const &delta,
                                                FunctionWrapper<argNum> &f) {
        RealVector<argNum> new_x = x;

        for (std::uint8_t i = 0; i < argNum; i++) {
            double f_base = f(new_x);
            new_x[i] += delta[i];
            double f_new = f(new_x);
            if (f_new >= f_base) {
                new_x[i] = x[i] - delta[i];
                f_new = f(new_x);
                if (f_new >= f_base) {
                    new_x[i] = x[i];
                }
            }
        }

        return new_x;
    }

public:
    AlgsForExtremes() = delete;

    template<std::uint8_t argNum>
    static RealVector<argNum> gradientMethod(FunctionWrapper<argNum> &f,
                                             FunctionWrapper<argNum, RealVector<argNum> > &df,
                                             double epsilon = 0.01, RealVector<argNum> x = DEFAULT_X,
                                             std::shared_ptr<std::vector<RealVector<argNum> > > chosedCoefs = nullptr) {
        RealVector<argNum> newX = x, df_x1;

        auto wrapper = FunctionWrapper<1>(
            [&df_x1, &x, &f](RealVector<1> const &coef) -> double {
                return f(x - df_x1 * coef[0]);
            });

        do {
            x = newX;
            df_x1 = df(x);

            double coef = goldenRatioMethod(wrapper, 0.0, 10.0, epsilon / 100.0);

            newX = x - df_x1 * coef;

            if (chosedCoefs != nullptr) {
                chosedCoefs->emplace_back(df_x1);
            }

        } while (x.normOfDifference(newX) >= epsilon);

        return newX;
    }

    template<std::uint8_t argNum>
    static RealVector<argNum> newtonMethod(FunctionWrapper<argNum> &f,
                                           FunctionWrapper<argNum, RealVector<argNum> > &df,
                                           FunctionWrapper<argNum, RealMatrix<argNum> > &hf,
                                           double epsilon = 0.01, RealVector<argNum> x = DEFAULT_X) {
        RealVector<argNum> newX = x;

        do {
            x = newX;
            newX = x - hf(x) * df(x);
        } while (x.normOfDifference(newX) >= epsilon);

        return newX;
    }


    template<std::uint8_t argNum>
    static RealVector<argNum> hookeJeevesMethod(FunctionWrapper<argNum> &f,
                                                double epsilon = 0.01, RealVector<argNum> x1 = DEFAULT_X,
                                                double stepSize = 1.0, std::uint32_t maxIterations = 100000) {
        RealVector<argNum> x2 = x1, delta(stepSize);

        for (std::uint32_t iter = 0; iter < maxIterations; iter++) {
            auto expPoint = exploratorySearch(x2, stepSize, delta, f);

            double f_exp = f(expPoint);
            if (f_exp < f(x2)) {
                RealVector<argNum> pattern;
                for (std::uint8_t i = 0; i < argNum; i++) {
                    pattern[i] = expPoint[i] + (expPoint[i] - x1[i]);
                }
                x1 = x2;
                if (f(pattern) < f_exp) {
                    x2 = pattern;
                } else {
                    x2 = expPoint;
                }
            } else {
                stepSize /= 2;
                for (std::uint8_t i = 0; i < argNum; i++) {
                    delta[i] = stepSize;
                }
                x2 = x1;
            }

            if (stepSize < epsilon / 10.0)
                break;
        }

        return x2;
    }

    static double goldenRatioMethod(FunctionWrapper<1> &f, double a, double b, double epsilon) {
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
