#pragma once

#include <vector>

template<typename T>
using Vector = std::vector<T>;

template<typename T>
using Matrix = std::vector<Vector<T>>;