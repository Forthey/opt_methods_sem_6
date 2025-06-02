#pragma once

#include <limits>
#include <string>
#include <vector>


// Класс Graph: хранит число вершин и матрицу смежности, умеет читать/писать JSON и генерировать связный граф
class Graph {
public:
    int N; // число вершин
    std::vector<std::vector<int> > adj; // матрица смежности

    // Конструктор: создает граф из N вершин с весами INF
    Graph(int n = 0) : N(n), adj(n, std::vector<int>(n, std::numeric_limits<int>::max())) {
    }

    // Загрузка графа из JSON-файла (ключ "adjacency_matrix")
    // Параметр: имя файла JSON.
    // Логика: парсит JSON и заполняет mat[i][j] = w. Вес 0 в файле означает отсутствие ребра (оставляем INF), диагональ = 0.
    bool loadFromJson(const std::string &filename);

    // Сохранение матрицы смежности в JSON-файл.
    // Вес INF (отсутствие ребра) сохраняется как 0 для читаемости.
    bool saveToJson(const std::string &filename) const;

    // Генерация случайного связного графа с N вершинами и случайными весами.
    // Логика: строим остовное дерево (чтобы гарантировать связность), затем случайно добавляем рёбра.
    static Graph generateRandomConnected(int N);
};
