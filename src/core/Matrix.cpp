#include "Matrix.hpp"
#include "MatrixError.hpp"
#include <cmath>
#include <iostream>

void Matrix::set(int row, int column, double value) {
    if (row < 0 || column < 0 || row >= rows || column >= columns) {
        throw MatrixError("Indeks jest poza macierzą");
    }
    values[row][column] = value;
}

void Matrix::print() const{
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            std::cout << values[r][c] << " ";
        }
        std::cout << "\n";
    }
}

Matrix::Matrix(int rows, int columns) {
    if (rows < 1 || columns < 1) {
        throw MatrixError("Wymiary macierzy muszą być większe od zera");
    }
    this->rows = rows;
    this->columns = columns;
    values.assign(rows, std::vector<double>(columns, 0));
}

void Matrix::userInputValues() {
    std::cout << "Podaj liczby, wiersz po wierszu.\n";
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            std::cout << "wiersz " << r + 1 << ", kolumna " << c + 1 << ": ";
            if (!(std::cin >> values[r][c])) {
                throw MatrixError("Oczekiwano liczby");
            }
        }
    }
}

Matrix Matrix::add(Matrix& other) {
    if (rows != other.rows || columns != other.columns) {
        throw MatrixError("Dodawanie macierzy wymaga takich samych wymiarów");
    }
    Matrix result(rows, columns);
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                result.values[r][c] = this->values[r][c] + other.values[r][c];
            }
        }
    return result;
}

Matrix Matrix::subtract(Matrix& other) {
    if (rows != other.rows || columns != other.columns) {
        throw MatrixError("Odejmowanie macierzy wymaga takich samych wymiarów");
    }
    Matrix result(rows, columns);
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                result.values[r][c] = this->values[r][c] - other.values[r][c];
            }
        }
    return result;
}

Matrix Matrix::multiply(Matrix& other) {
    if (columns != other.rows) {
        throw MatrixError("Mnożenie macierzy wymaga, żeby liczba kolumn pierwszej macierzy była równa liczbie wierszy drugiej macierzy");
    }
    Matrix result(rows, other.columns);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < other.columns; c++) {
            for (int k = 0; k < columns; k++) {
                result.values[r][c] += this->values[r][k] * other.values[k][c];
            }
        }
    }
    return result;
}

Matrix Matrix::divide(Matrix& other) {
    if (other.rows != other.columns) {
        throw MatrixError("Dzielenie przez macierz wymaga, żeby macierz była kwadratowa");
    }
    if (columns != other.rows) {
        throw MatrixError("Dzielenie wymaga, żeby liczba kolumn pierwszej macierzy była równa rozmiarowi drugiej");
    }
    if (std::fabs(other.determinant()) < 1e-10) {
        throw MatrixError("Dzielenie przez macierz o wyznaczniku równym 0 jest niemożliwe");
    }
    /*
    Matrix result(rows, columns);
    Matrix new_other = other.inverse();
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            for (int k = 0; k < columns; k++) {
                result.values[r][c] += this->values[r][k] * new_other.values[k][c];
            }
        }
    }
    return result;
    */
    Matrix inverted = other.inverse();
    return multiply(inverted);
}

Matrix Matrix::transpose() {
    Matrix result(columns, rows);
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                result.values[c][r] = this->values[r][c];
            }
        }
    return result;
}

Matrix Matrix::identity(){
    if (rows != columns) {
        throw MatrixError("Macierz jednostkowa wymaga, żeby macierz była kwadratowa");
    }
    Matrix result(rows, columns);
        for (int r = 0; r < rows; r++) {
            result.values[r][r] = 1;
        }
    return result;
}
    
Matrix Matrix::inverse() {
    if (rows != columns) {
        throw MatrixError("Inwersja macierzy wymaga, żeby macierz była kwadratowa");
    }
    if (std::fabs(determinant()) < 1e-10) {
        throw MatrixError("Inwersja macierzy o wyznaczniku równym 0 jest niemożliwa");
    }
    Matrix result(rows, columns);
    const int n = rows;
    std::vector<std::vector<double>> aug(n, std::vector<double>(2 * n, 0));
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            aug[r][c] = values[r][c];
        }
        aug[r][n + r] = 1;
    }

    for (int col = 0; col < n; col++) {
        int pivot = col;
        double best = aug[col][col];
        if (best < 0) {
            best = -best;
        }
        for (int r = col + 1; r < n; r++) {
            double mag = aug[r][col];
            if (mag < 0) {
                mag = -mag;
            }
            if (mag > best) {
                best = mag;
                pivot = r;
            }
        }
        if (best < 1e-10) {
            throw MatrixError("Macierz jest osobliwa, nie ma odwrotności");
        }
        if (pivot != col) {
            std::vector<double> tmp = aug[col];
            aug[col] = aug[pivot];
            aug[pivot] = tmp;
        }

        double pivotValue = aug[col][col];
        for (int c = 0; c < 2 * n; c++) {
            aug[col][c] /= pivotValue;
        }

        for (int r = 0; r < n; r++) {
            if (r == col) {
                continue;
            }
            double factor = aug[r][col];
            for (int c = 0; c < 2 * n; c++) {
                aug[r][c] -= factor * aug[col][c];
            }
        }
    }

    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            result.values[r][c] = aug[r][n + c];
        }
    }
    return result;
}


double Matrix::determinant() const { 
    if (rows != columns) {
        throw MatrixError("Wyznacznik macierzy wymaga, żeby macierz była kwadratowa");
    }
    if (rows == 1) {
        return values[0][0];
    }
    if (rows == 2) {
        return values[0][0] * values[1][1] - values[0][1] * values[1][0];
    }
    if (rows == 3) {
        double plusy = 0, minusy = 0;
        for (int i = 0; i < 3; i++) {
            plusy += values[0][i] * values[1][(i + 1) % 3] * values[2][(i + 2) % 3];
            minusy += values[2][i] * values[1][(i + 1) % 3] * values[0][(i + 2) % 3];
        }
        return plusy - minusy;
    }

    // 4x4 i wieksze, takze 100x100: kopia, zera pod przekatna, iloczyn przekatnej.
    std::vector<std::vector<double>> a = values;
    double sign = 1;
    for (int col = 0; col < rows; col++) {
        int pivot = col;
        double best = a[col][col];
        if (best < 0) {
            best = -best;
        }
        for (int r = col + 1; r < rows; r++) {
            double mag = a[r][col];
            if (mag < 0) {
                mag = -mag;
            }
            if (mag > best) {
                best = mag;
                pivot = r;
            }
        }
        if (best < 1e-10) {
            return 0;
        }
        if (pivot != col) {
            std::vector<double> tmp = a[col];
            a[col] = a[pivot];
            a[pivot] = tmp;
            sign = -sign;
        }
        for (int r = col + 1; r < rows; r++) {
            double factor = a[r][col] / a[col][col];
            for (int c = col; c < rows; c++) {
                a[r][c] -= factor * a[col][c];
            }
        }
    }

    double det = sign;
    for (int i = 0; i < rows; i++) {
        det *= a[i][i];
    }
    return det;
}

double Matrix::trace() {
    if (rows != columns) {
        throw MatrixError("Ślad macierzy wymaga, żeby macierz była kwadratowa");
    }
    double sum = 0;
    for (int r = 0; r < rows; r++) {
        sum += values[r][r];
    }
    return sum;    
}