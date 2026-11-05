#include "Operations.hpp"
#include "MatrixError.hpp"
#include <cmath>

const double EPSILON = 1e-10;

bool isZero(double value) {
    if (std::fabs(value) < EPSILON) {
        return true;
    }
    return false;
}

void swapRows(Matrix& matrix, int rowA, int rowB) {
    for (int column = 0; column < matrix.getColumns(); column++) {
        double valueA = matrix.get(rowA, column);
        double valueB = matrix.get(rowB, column);
        matrix.set(rowA, column, valueB);
        matrix.set(rowB, column, valueA);
    }
}

void divideRow(Matrix& matrix, int row, double divisor) {
    for (int column = 0; column < matrix.getColumns(); column++) {
        double oldValue = matrix.get(row, column);
        double newValue = oldValue / divisor;
        matrix.set(row, column, newValue);
    }
}

void subtractRowMultiple(Matrix& matrix, int targetRow, int sourceRow, double factor) {
    for (int column = 0; column < matrix.getColumns(); column++) {
        double targetValue = matrix.get(targetRow, column);
        double sourceValue = matrix.get(sourceRow, column);
        double newValue = targetValue - factor * sourceValue;
        matrix.set(targetRow, column, newValue);
    }
}

int findPivotRow(const Matrix& matrix, int column, int startRow) {
    int bestRow = startRow;
    double bestValue = std::fabs(matrix.get(startRow, column));

    for (int row = startRow + 1; row < matrix.getRows(); row++) {
        double value = std::fabs(matrix.get(row, column));
        if (value > bestValue) {
            bestValue = value;
            bestRow = row;
        }
    }
    return bestRow;
}

bool isRowZero(const Matrix& matrix, int row) {
    for (int column = 0; column < matrix.getColumns(); column++) {
        if (!isZero(matrix.get(row, column))) {
            return false;
        }
    }
    return true;
}

Matrix joinSideBySide(const Matrix& left, const Matrix& right) {
    int rowCount = left.getRows();
    int leftWidth = left.getColumns();
    int rightWidth = right.getColumns();
    Matrix result(rowCount, leftWidth + rightWidth);

    for (int row = 0; row < rowCount; row++) {
        for (int column = 0; column < leftWidth; column++) {
            result.set(row, column, left.get(row, column));
        }
        for (int column = 0; column < rightWidth; column++) {
            result.set(row, leftWidth + column, right.get(row, column));
        }
    }
    return result;
}

Matrix copyColumns(const Matrix& matrix, int firstColumn, int howMany) {
    Matrix result(matrix.getRows(), howMany);
    for (int row = 0; row < matrix.getRows(); row++) { 
        for (int column = 0; column < howMany; column++) {
            double value = matrix.get(row, firstColumn + column);
            result.set(row, column, value);
        }
    }
    return result;
}

bool leftPartIsIdentity(const Matrix& matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            double expected;
            if (row == column) {
                expected = 1.0;
            } else {
                expected = 0.0;
            }
            double difference = matrix.get(row, column) - expected;
            if (!isZero(difference)) {
                return false;
            }
        }
    }
    return true;
}

Matrix rref(const Matrix& original) {
    Matrix matrix = original;
    int rowCount = matrix.getRows();
    int columnCount = matrix.getColumns();
    int pivotRow = 0;

    for (int column = 0; column < columnCount; column++) {
        if (pivotRow == rowCount) {
            break;
        }

        int bestRow = findPivotRow(matrix, column, pivotRow);
        double bestValue = matrix.get(bestRow, column);

        if (isZero(bestValue)) {
        } else {
            swapRows(matrix, pivotRow, bestRow);

            double pivotValue = matrix.get(pivotRow, column);
            divideRow(matrix, pivotRow, pivotValue);

            for (int row = 0; row < rowCount; row++) {
                if (row != pivotRow) {
                    double factor = matrix.get(row, column);
                    subtractRowMultiple(matrix, row, pivotRow, factor);
                }
            }
            pivotRow++;
        }
    }
    return matrix;
}

int rank(const Matrix& original) {
    Matrix reduced = rref(original);
    int count = 0;
    for (int row = 0; row < reduced.getRows(); row++) {
        if (!isRowZero(reduced, row)) {
            count++;
        }
    }
    return count;
}

Matrix inv(const Matrix& original) {
    if (original.getRows() != original.getColumns()) {
        throw MatrixError("Inwersja macierzy wymaga, żeby macierz była kwadratowa");
    }
    int size = original.getRows();

    Matrix identity = Matrix::identity(size);
    Matrix joined = joinSideBySide(original, identity);
    Matrix reduced = rref(joined);

    if (!leftPartIsIdentity(reduced, size)) {
        throw MatrixError("Macierz jest osobliwa, nie ma odwrotności");
    }

    return copyColumns(reduced, size, size);
}

Matrix solve(const Matrix& a, const Matrix& b) {
    if (a.getRows() != a.getColumns()) {
        throw MatrixError("Rozwiązywanie układu wymaga kwadratowej macierzy A");
    }
    if (a.getRows() != b.getRows()) {
        throw MatrixError("Macierze A i B muszą mieć tyle samo wierszy");
    }
    int size = a.getRows();

    Matrix joined = joinSideBySide(a, b);
    Matrix reduced = rref(joined);

    if (!leftPartIsIdentity(reduced, size)) {
        throw MatrixError("Brak jednoznacznego rozwiązania");
    }

    return copyColumns(reduced, size, b.getColumns());
}

Matrix power(const Matrix& original, int exponent) {
    if (original.getRows() != original.getColumns()) {
        throw MatrixError("Potęgowanie wymaga macierzy kwadratowej");
    }

    if (exponent < 0) {
        Matrix inverted = inv(original);
        return power(inverted, -exponent);
    }

    Matrix result = Matrix::identity(original.getRows());
    for (int i = 0; i < exponent; i++) {
        result = result.multiply(original);
    }
    return result;
}

double det(const Matrix& original) {
    return original.determinant();
}

double trace(const Matrix& original) {
    return original.trace();
}
