#include <iostream>
#include <ostream>

#include "SlackFormTask.h"
#include "LinearTask.h"


double operator*(std::vector<double> const& v1, std::vector<double> const& v2) {
    double sum = 0;
    for (int i = 0; i < v1.size(); i++) {
        sum += v1[i] * v2[i];
    }
    return sum;
}

void printResult(double result, std::vector<double> const& x) {
    std::cout << std::format("z = {}\n" , result);
    for (std::size_t i = 0; i < x.size(); i++) {
        std::cout << std::format("x{} = {}\t", i + 1, x[i]);
    }
    std::cout << std::endl;
}


int main() {
    LinearTask linearTask(
        Matrix({
            {-1, -1, -1, -1, -1},
            {1, 2, 3, 2, 1}
        }),
        Matrix({
            {1, 2, 3, 4, 5},
            {4, 1, 2, 3, 5},
            {3, 4, 1, 5, 2}
        }),
        {-10, 20},
        {30, 40, 50},
        {-2, -3, -4, -5, -1},
        0.0,
        {5}
    ), dualTask = linearTask.getDualTask();

    SlackFormTask slackFormTask(linearTask.getA(), linearTask.getB(), linearTask.getC(), linearTask.getV()),
        slackFormDualTask(dualTask.getA(), dualTask.getB(), dualTask.getC(), dualTask.getV());

    std::cout << "---- Task ----\n";
    std::cout << std::format("rank: {}\n", linearTask.getA().getRank());
    auto result = linearTask.getTrueValues(slackFormTask.simplex());
    printResult(result * linearTask.getC() + linearTask.getV(), result);

    std::cout << "---- Dual task ----\n";
    std::cout << std::format("rank: {}\n", dualTask.getA().getRank());
    result = dualTask.getTrueValues(slackFormDualTask.simplex());
    printResult(result * dualTask.getC() + dualTask.getV(), result);
}