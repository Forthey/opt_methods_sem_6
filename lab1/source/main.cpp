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


int main() {
    LinearTask linearTask(
        Matrix({
            {1, 0, 1, 1, 0},
            {0, -1, -1, 0, 1}
        }),
        Matrix({
            {1, 1, 0, 0, 1},
            {2, 0, 1, 0, 0},
            {0, 1, 0, 1, 0}
        }),
        {10, -2},
        {6, 8, 7},
        {2, 1, 3, 4, 1},
        0.0,
        {5}
    );

    SlackFormTask slackFormTask(linearTask.getA(), linearTask.getB(), linearTask.getC(), linearTask.getV());

    auto result = linearTask.getTrueValues(slackFormTask.simplex());
    std::cout << result * linearTask.getC() + linearTask.getV() << std::endl;
}