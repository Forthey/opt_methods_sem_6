#include <Windows.h>

#include <iostream>
#include <ostream>

#include "StandardTask.h"
#include "SlackFormTask.h"
#include "Task.h"


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
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    setlocale(LC_ALL, "ru_RU.UTF-8");

    Task task("other");

    task.print();

    StandardTask standardTask(task);

    standardTask.print();

    SlackFormTask slackFormTask(standardTask);

    slackFormTask.print();
    std::vector<double> solution;

    auto flex = slackFormTask.simplex(solution);
    auto result = standardTask.getTrueValues(solution);
    printResult(result * standardTask.getTargetFunction(), result);
    std::cout << flex << '\n';
}