#include <iostream>
#include <Windows.h>

#include "utility/print_table.h"
#include "TransportTask.h"


int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    std::cout << "Введите имя json файла с данными транспортной задачи\n>";
    std::string filename;
    std::cin >> filename;

    TransportTask transportTask(filename);

    std::cout << "\nПоставщики" << std::endl;
    printRow(transportTask.getSuppliers());
    std::cout << "\nПотребители" << std::endl;
    printRow(transportTask.getConsumers());
    std::cout << "\nТаблица стоимости" << std::endl;
    printTable(transportTask.getPathCosts());

    std::cout << "\nОпорный план" << std::endl;
    auto basicPlan = transportTask.northwestCornerMethod();
    printTable(basicPlan);
    std::cout << "Стоимость: " << transportTask.calculateCosts(basicPlan) << std::endl;

    std::cout << "\nОптимальные планы" << std::endl;
    auto optimalPlan = transportTask.potentialsMethod(basicPlan);
    printTable(transportTask.restorePlan(optimalPlan));
    std::cout << "Стоимость: " << transportTask.calculateCosts(optimalPlan) << std::endl;
}