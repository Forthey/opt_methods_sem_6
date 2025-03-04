#include "print_table.h"

void printRow(std::vector<std::int64_t> const& row) {
    for (std::size_t i = 0; i < row.size(); i++) {
        std::cout << row[i] << (i == row.size() - 1 ? "\n" : "\t");
    }
}

void printTable(std::vector<std::vector<std::int64_t>> const& table) {
    for (auto& row : table) {
        printRow(row);
    }
}