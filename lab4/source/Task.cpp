#include "Task.h"

#include <iostream>
#include <cmath>

#include "alg/AlgsForExtremes.h"


RealVector<2> const Task::trueMin(-0.1571849514838140095869566960, -1.157184951483814009586956696);

void printTable(std::string const& title, std::vector<std::vector<std::string> > const &table) {
    static std::mutex mtx;

    std::vector<size_t> column_widths(table[0].size(), 0);
    for (std::size_t i = 0; i < table[0].size(); i++) {
        for (std::size_t j = 0; j < table.size(); j++) {
            column_widths[i] = std::max(column_widths[i], table[j][i].size());
        }
    }

    mtx.lock();

    std::cout << title << std::endl;
    for (const auto &row: table) {
        for (size_t i = 0; i < row.size(); ++i) {
            std::cout << std::format("{:<{}}", row[i], column_widths[i] + 5);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    mtx.unlock();
}

double Task::function(RealVector<2> const &x) {
    return std::exp(x[0] - 1.0) + x[0] * x[0] + std::exp(x[1]) + (x[1] + 1.0) * (x[1] + 1.0);
}

RealVector<2> Task::functionGradient(RealVector<2> const &x) {
    return RealVector(std::exp(x[0] - 1.0) + 2.0 * x[0], std::exp(x[1]) + 2 * (x[1] + 1.0));
}

RealMatrix<2> Task::functionInvertedHessian(RealVector<2> const &x) {
    double
            a = std::exp(x[0] - 1.0) + 2.0,
            b = 0.0,
            c = 0.0,
            d = std::exp(x[1]) + 2.0;
    double coef = 1 / (a * d - b * c);
    return RealMatrix(
        RealVector(d, -b) * coef,
        RealVector(-c, a) * coef
    );
}

void Task::gradientMethodCalcs() {
    auto epsilons = {0.1, 0.01, 0.001};
    FunctionWrapper<2, double> f(function);
    FunctionWrapper<2, RealVector<2> > df(functionGradient);

    std::vector<std::vector<std::string> > table = {{"e", "x", "dx", "n_f", "n_df"}};

    for (auto &epsilon: epsilons) {
        auto result = AlgsForExtremes::gradientMethod<2>(f, df, epsilon);
        table.emplace_back(std::vector{
            RealVector<>::toStringWithPrecision(epsilon, epsilon * 10.0),
            result.toString(epsilon),
            (result - trueMin).toString(epsilon),
            std::to_string(f.getUseCount()),
            std::to_string(df.getUseCount())
        });

        f.resetUseCount();
        df.resetUseCount();
    }


    printTable("Градиентный метод", table);
}

void Task::newtonMethodCalcs() {
    auto epsilons = {0.1, 0.01, 0.001};
    FunctionWrapper<2, double> f(function);
    FunctionWrapper<2, RealVector<2> > df(functionGradient);
    FunctionWrapper<2, RealMatrix<2> > hf(functionInvertedHessian);

    std::vector<std::vector<std::string> > table = {{"e", "x", "dx", "n_f", "n_df", "n_hf"}};

    for (auto &epsilon: epsilons) {
        auto result = AlgsForExtremes::newtonMethod<2>(f, df, hf, epsilon);
        table.emplace_back(std::vector{
            RealVector<>::toStringWithPrecision(epsilon, epsilon * 10.0),
            result.toString(epsilon),
            (result - trueMin).toString(epsilon),
            std::to_string(f.getUseCount()),
            std::to_string(df.getUseCount()),
            std::to_string(hf.getUseCount())
        });

        f.resetUseCount();
        df.resetUseCount();
        hf.resetUseCount();
    }

    printTable("Метод Ньютона", table);
}

void Task::hookeJeevesCalcs() {
    auto epsilons = {0.1, 0.01, 0.001};
    FunctionWrapper<2, double> f(function);

    std::vector<std::vector<std::string> > table = {{"e", "x", "dx", "n_f"}};

    for (auto &epsilon: epsilons) {
        auto result = AlgsForExtremes::hookeJeevesMethod<2>(f, epsilon);
        table.emplace_back(std::vector{
            RealVector<>::toStringWithPrecision(epsilon, epsilon * 10.0),
            result.toString(epsilon),
            (result - trueMin).toString(epsilon),
            std::to_string(f.getUseCount()),
        });

        f.resetUseCount();
    }

    printTable("Метод Хука-Дживса", table);
}

void Task::checkForOrthogonalityInGradient() {
    auto epsilon = 0.000001;
    FunctionWrapper<2, double> f(function);
    FunctionWrapper<2, RealVector<2> > df(functionGradient);

    auto coefs = std::make_shared<std::vector<RealVector<2>>>();
    auto result = AlgsForExtremes::gradientMethod<2>(f, df, epsilon, DEFAULT_X, coefs);

    std::vector<std::vector<std::string> > table = {{"k", "k + 1", "(k) * (k + 1)"}};

    for (std::size_t i = 0; i < coefs->size() - 1; ++i) {
        table.emplace_back(std::vector{
            coefs->at(i).toString(epsilon),
            coefs->at(i + 1).toString(epsilon),
            RealVector<>::toStringWithPrecision(coefs->at(i) * coefs->at(i + 1), epsilon)
        });
    }

    printTable("Проверка ортогональности звеньев градиентной ломаной в методе Ньютона для e = " + std::to_string(epsilon), table);
}

void Task::gradientWithNewtonCalcs() {
    auto epsilons = {0.1, 0.01, 0.001};
    FunctionWrapper<2, double> f(function);
    FunctionWrapper<2, RealVector<2> > df(functionGradient);
    FunctionWrapper<2, RealMatrix<2> > hf(functionInvertedHessian);

    std::vector<std::vector<std::string> > table = {{"e", "x", "dx", "n_f", "n_df", "n_hf"}};

    for (auto &epsilon: epsilons) {
        auto result = AlgsForExtremes::newtonMethod<2>(f, df, hf, epsilon,
                                                       AlgsForExtremes::gradientMethod<2>(f, df, 0.5));
        table.emplace_back(std::vector{
            RealVector<>::toStringWithPrecision(epsilon, epsilon * 10.0),
            result.toString(epsilon),
            (result - trueMin).toString(epsilon),
            std::to_string(f.getUseCount()),
            std::to_string(df.getUseCount()),
            std::to_string(hf.getUseCount())
        });

        f.resetUseCount();
        df.resetUseCount();
        hf.resetUseCount();
    }

    printTable("Градиентный метод + метод Ньютона", table);
}

void Task::hookeJeevesWithNewtonCalcs() {
    auto epsilons = {0.1, 0.01, 0.001};
    FunctionWrapper<2, double> f(function);
    FunctionWrapper<2, RealVector<2> > df(functionGradient);
    FunctionWrapper<2, RealMatrix<2> > hf(functionInvertedHessian);

    std::vector<std::vector<std::string> > table = {{"e", "x", "dx", "n_f", "n_df", "n_hf"}};

    for (auto &epsilon: epsilons) {
        auto result = AlgsForExtremes::newtonMethod<2>(f, df, hf, epsilon,
                                                       AlgsForExtremes::hookeJeevesMethod<2>(f,  0.5));
        table.emplace_back(std::vector{
            RealVector<>::toStringWithPrecision(epsilon, epsilon * 10.0),
            result.toString(epsilon),
            (result - trueMin).toString(epsilon),
            std::to_string(f.getUseCount()),
            std::to_string(df.getUseCount()),
            std::to_string(hf.getUseCount())
        });

        f.resetUseCount();
        df.resetUseCount();
        hf.resetUseCount();
    }

    printTable("Метод Хука-Дживса + метод Ньютона", table);
}
