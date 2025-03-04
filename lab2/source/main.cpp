#include <Windows.h>

#include "utility/print_table.h"
#include "TransportTask.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    std::cout << "Введите имя файла с данными транспортной задачи\n>";
    std::string filename;
    std::cin >> filename;

    TransportTask transportTask(filename);

    std::cout << "\nПоставщики" << std::endl;
    printRow(transportTask.suppliers);
    std::cout << "\nПотребители" << std::endl;
    printRow(transportTask.consumers);
    std::cout << "\nТаблица стоимости" << std::endl;
    printTable(transportTask.pathCosts);

    std::cout << "\nОпорный план" << std::endl;
    printTable(transportTask.northwestCornerMethod());
}