#include <cmath>
#include <windows.h>

#include "BaseTask.h"
#include "PlotTask.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    FunctionWrapper wrapper(
        [](double x) -> double { return 0.5 * x * x - std::sin(x); },
        [](double x) -> bool { return 0.0 <= x && x <= 1.0; }
    );

    BaseTask::doTask(wrapper);

    PlotTask::doTask(wrapper, -8, -1, 100);

    return 0;
}
