#include "Matrix.hpp"

#include <iostream>

Matrix::Matrix(int rows, int columns) {
    this->rows = rows;
    this->columns = columns;
    values.assign(rows, std::vector<double>(columns, 0));
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
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns; c++) {
                result.values[r][c] = this->values[r][c] - other.values[r][c];
            }
        }
    return result;
}

Matrix Matrix::multiply(Matrix& other) {
    Matrix result(rows, other.columns);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < other.columns; c++) {
            for (int k = 0; k < other.columns; k++) {
                result.values[r][c] += this->values[r][k] * other.values[k][c];
            }
        }
    }
    return result;
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
    Matrix result(rows, columns);

    return result;
}

Matrix Matrix::inverse() {
    Matrix result(rows, columns);
        for (int r = 0; r < rows; r++) {
            result.values[r][r] = 1;
        }
    return result;
}


double Matrix::determinant() const { 
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
    int sum = 0;
    for (int r = 0; r < rows; r++) {
        sum += values[r][r];
    }
    return sum;    
}