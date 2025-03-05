#pragma once

#include <vector>

template<typename T>
using Row = std::vector<T>;

template<typename T>
using Table = std::vector<Row<T>>;