#include <thread>
#include <vector>
#include <Windows.h>

#include "Task.h"


int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    std::vector<std::thread> tasks;

    Task::printFunction();

    tasks.emplace_back(Task::gradientMethodCalcs);
    tasks.emplace_back(Task::newtonMethodCalcs);
    tasks.emplace_back(Task::hookeJeevesCalcs);
    tasks.emplace_back(Task::checkForOrthogonalityInGradient);
    tasks.emplace_back(Task::gradientWithNewtonCalcs);
    tasks.emplace_back(Task::hookeJeevesWithNewtonCalcs);

    for (auto& t : tasks) {
        t.join();
    }

    return EXIT_SUCCESS;
}
