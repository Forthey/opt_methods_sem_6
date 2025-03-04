#include "print_table.h"

#include <iostream>

void printRow(Row<double> const& row) {
    for (std::size_t i = 0; i < row.size(); i++) {
        std::cout << row[i] << (i == row.size() - 1 ? "\n" : "\t");
    }
}

void printTable(Table<double> const& table) {
    for (auto& row : table) {
        printRow(row);
    }
}