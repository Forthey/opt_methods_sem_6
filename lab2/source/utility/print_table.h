#pragma once
#include <iostream>
#include <vector>


void printRow(std::vector<std::size_t> const& row) {
    for (std::size_t i = 0; i < row.size(); i++) {
        std::cout << row[i] << (i == row.size() - 1 ? "\n " : " ");
    }
}

void printTable(std::vector<std::vector<std::size_t>> const& table) {
    for (auto& row : table) {
        printRow(row);
    }
}