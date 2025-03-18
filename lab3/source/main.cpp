#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <windows.h>

// #include <nlohmann/json.hpp>

#include "BaseTask.h"
#include "alg/AlgsForExtremes.h"


// void doCalcs(std::string const &taskFileName) {
//     std::string const path = std::format("tasks/{}.jsom", taskFileName);
//     std::ifstream jsonFile(path);
//
//     if (!jsonFile.is_open()) {
//         std::cerr << "Error opening file " << path << std::endl;
//         return;
//     }
//
//     auto json = nlohmann::json::parse(jsonFile);
//     if (!json.contains("epsilons")) {
//         std::cerr << "Error parsing json file: 'epsilons' field not found" << std::endl;
//         return;
//     }
//
//     try {
//         std::vector<double> epsilons = json["epsilons"].get<std::vector<double> >();
//
//         for (std::size_t i = 0; i < epsilons.size(); i++) {
//         }
//     } catch (std::exception const &e) {
//         std::cerr << "Error parsing json file: " << e.what() << std::endl;
//     }
// }


int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    FunctionWrapper wrapper(
        [](double x) -> double { return 0.5 * x * x - std::sin(x); },
        [](double x) -> bool { return 0.0 <= x && x <= 1.0; }
    );

    BaseTask::doTask(wrapper);

    return 0;
}
