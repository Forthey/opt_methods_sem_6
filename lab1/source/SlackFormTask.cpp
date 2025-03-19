#include "SlackFormTask.h"

#include <iostream>
#include <memory>

#include "utility/print_matrix.h"

/**
 *
 * @param l Индекс выводимой переменной из множества B (l в книге)
 * @param e Индекс вводимой переменной из множества N (e в книге)
 */
void SlackFormTask::pivot(int& row, int& col) {
    double pivotVal = tableau[row][col];
        int totalCols = tableau[0].size();

        // Нормализуем строку row
        for (int j = 0; j < totalCols; j++) {
            tableau[row][j] /= pivotVal;
        }
        // Обнуляем столбец col в остальных строках
        for (int i = 0; i < tableau.size(); i++) {
            if (i != row) {
                double factor = tableau[i][col];
                for (int j = 0; j < totalCols; j++) {
                    tableau[i][j] -= factor * tableau[row][j];
                }
            }
        }
        // Обновляем базисную переменную для строки row
        basis[row] = col;
}

double SlackFormTask::simplex(std::vector<double> &solution) {
    int rows = tableau.size();
    int cols = tableau[0].size();

    while (true) {
        // Находим опорный столбец: выбираем первый столбец в строке цели с отрицательным коэффициентом
        int pivotCol = -1;
        for (int j = 0; j < cols - 1; j++) {
            if (tableau[rows - 1][j] < -EPS) {
                pivotCol = j;
                break;
            }
        }
        // Если отрицательных коэффициентов нет – оптимальное решение найдено
        if (pivotCol == -1)
            break;


        // Находим опорную строку методом минимального отношения (правый столбец делим на положительный элемент в pivotCol)
        int pivotRow = -1;
        double minRatio = std::numeric_limits<double>::infinity();
        for (int i = 0; i < rows; i++) {
            if (tableau[i][pivotCol] > EPS) {
                double ratio = tableau[i].back() / tableau[i][pivotCol];
                if (ratio < minRatio) {
                    minRatio = ratio;
                    pivotRow = i;
                }
            }
        }
        // Если подходящая строка не найдена, задача неограничена
        if (pivotRow == -1)
            throw std::runtime_error("Решение неограничено");

        // Выполняем поворот
        pivot(pivotRow, pivotCol);
    }

    // Извлекаем решение для исходных переменных (индексы от 0 до m-1)
    solution.assign(m, 0);
    for (int i = 0; i < n; i++) {
        if (basis[i] < m) {
            solution[basis[i]] = tableau[i].back();
        }
    }
    return tableau[rows - 1].back();
}

SlackFormTask::SlackFormTask(StandardTask const& task) {
        n = task.getB().size();
        m = task.getTargetFunction().size();
        int totalCols = m + n + 1; // исходные переменные + слэк-переменные + свободный член
        tableau.assign(n + 1, std::vector<double>(totalCols, 0));

        // Заполнение строк ограничений:
        // Каждая строка имеет коэффициенты исходных переменных, затем единичная матрица для слэк-переменных и правую часть b[i]
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                tableau[i][j] = task.getA()[i][j];
            }
            tableau[i][m + i] = 1;   // коэффициент для слэк-переменной i
            tableau[i].back() = task.getB()[i];  // правая часть
        }

        // Заполнение строки цели (нижняя строка таблицы):
        // Здесь коэффициенты исходных переменных равны -c[j] (так как мы решаем задачу максимизации)
        for (int j = 0; j < m; j++) {
            tableau[n][j] = -task.getTargetFunction()[j];
        }

        // Изначально базисными переменными являются слэк-переменные с индексами m, m+1, ..., m+n-1
        basis.resize(n);
        for (int i = 0; i < n; i++) {
            basis[i] = m + i;
        }
}

void SlackFormTask::print() {
    std::cout << "\nЗадача ЛП в канонической форме" << std::endl;
    printTargetFunction(tableau[0]);
    for (std::size_t i = 0; i < tableau.size(); ++i) {
        for (std::size_t j = 0; j < tableau[i].size(); ++j) {
            std::cout << std::format("{}\t", tableau[i][j]);
        }
        std::cout << std::endl;
    }
}

