#include "Matrix.hpp"

Matrix::Matrix(int rows, int columns) {
    this->rows = rows;
    this->columns = columns;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            values[r][c] = 0;
        }
    }
}

void Matrix::userInputValues() {
    std::cout << "Podaj liczby, wiersz po wierszu.\n";
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            std::cout << "wiersz " << r + 1 << ", kolumna " << c + 1 << ": ";
            std::cin >> values[r][c];
        }
    }
}

Matrix Matrix::add(Matrix& other) {
    Matrix result(rows, columns);
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                result.values[r][c] = this->values[r][c] + other.values[r][c];
            }
        }
    return result;
}

Matrix Matrix::subtract(Matrix& other) {
    
    Matrix result(rows, columns);
    return result;
}

Matrix Matrix::multiply(Matrix& other) {
    Matrix result(rows, columns);
    return result;
}

Matrix Matrix::transpose() {
    Matrix result(columns, rows);
    return result;
}

Matrix Matrix::inverse() {
    Matrix result(rows, columns);
    return result;
}
