#include <Windows.h>

#include "utility/print_table.h"
#include "TransportTask.h"

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::setlocale(LC_ALL, "ru_RU.UTF-8");

    std::cout << "Введите имя файла с данными транспортной задачи: ";
    std::string filename;
    std::cin >> filename;

    TransportTask transportTask(filename);

    printRow(transportTask.suppliers);
    printRow(transportTask.consumers);
    printTable(transportTask.pathCosts);
}