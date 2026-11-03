#include "Operations.hpp"
#include "MatrixError.hpp"
#include <cmath>

// Liczby mniejsze od tej traktujemy jak zero.
// (Po wielu dzieleniach "0" często wychodzi jako 0.0000000000001.)
const double EPSILON = 1e-10;

// ===== Funkcje pomocnicze (używane tylko w tym pliku) =====

bool isZero(double value) {
    // std::fabs usuwa minus: fabs(-5) = 5, fabs(5) = 5
    if (std::fabs(value) < EPSILON) {
        return true;
    }
    return false;
}

// Ruch 1: zamiana dwóch wierszy miejscami
void swapRows(Matrix& matrix, int rowA, int rowB) {
    // Idziemy po każdej kolumnie i zamieniamy w niej dwie wartości
    for (int column = 0; column < matrix.getColumns(); column++) {
        double valueA = matrix.get(rowA, column);
        double valueB = matrix.get(rowB, column);
        matrix.set(rowA, column, valueB);
        matrix.set(rowB, column, valueA);
    }
}

// Ruch 2: podzielenie całego wiersza przez liczbę
void divideRow(Matrix& matrix, int row, double divisor) {
    for (int column = 0; column < matrix.getColumns(); column++) {
        double oldValue = matrix.get(row, column);
        double newValue = oldValue / divisor;
        matrix.set(row, column, newValue);
    }
}
z
// Ruch 3: targetRow = targetRow - factor * sourceRow
void subtractRowMultiple(Matrix& matrix, int targetRow, int sourceRow, double factor) {
    for (int column = 0; column < matrix.getColumns(); column++) {
        double targetValue = matrix.get(targetRow, column);
        double sourceValue = matrix.get(sourceRow, column);
        double newValue = targetValue - factor * sourceValue;
        matrix.set(targetRow, column, newValue);
    }
}

// Szuka wiersza z największą liczbą (bez minusa) w danej kolumnie.
// Patrzy tylko od startRow w dół, bo wiersze wyżej mają już swój element główny.
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

// Czy cały wiersz składa się z (prawie) zer?
bool isRowZero(const Matrix& matrix, int row) {
    for (int column = 0; column < matrix.getColumns(); column++) {
        if (!isZero(matrix.get(row, column))) {
            return false;      // znaleziono liczbę różną od zera
        }
    }
    return true;               // wszystkie liczby były zerami
}

// Skleja dwie macierze obok siebie: [left | right]
Matrix joinSideBySide(const Matrix& left, const Matrix& right) {
    int rowCount = left.getRows();
    int leftWidth = left.getColumns();
    int rightWidth = right.getColumns();
    Matrix result(rowCount, leftWidth + rightWidth);

    for (int row = 0; row < rowCount; row++) {
        // kopiujemy lewą macierz do lewej części
        for (int column = 0; column < leftWidth; column++) {
            result.set(row, column, left.get(row, column));
        }
        // kopiujemy prawą macierz do prawej części (przesuniętą o leftWidth)
        for (int column = 0; column < rightWidth; column++) {
            result.set(row, leftWidth + column, right.get(row, column));
        }
    }
    return result;
}

// Wycina howMany kolumn, zaczynając od kolumny firstColumn
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

// Czy lewa część size x size to macierz jednostkowa (1 na przekątnej, 0 poza nią)?
bool leftPartIsIdentity(const Matrix& matrix, int size) {
    for (int row = 0; row < size; row++) {
        for (int column = 0; column < size; column++) {
            double expected;
            if (row == column) {
                expected = 1.0;    // na przekątnej
            } else {
                expected = 0.0;    // wszędzie indziej
            }
            double difference = matrix.get(row, column) - expected;
            if (!isZero(difference)) {
                return false;
            }
        }
    }
    return true;
}

// ===== Główne funkcje (zadeklarowane w Operations.hpp) =====

Matrix rref(const Matrix& original) {
    Matrix matrix = original;          // pracujemy na kopii, nigdy na oryginale
    int rowCount = matrix.getRows();
    int columnCount = matrix.getColumns();
    int pivotRow = 0;                  // tu trafi następny element główny (ta "1")

    for (int column = 0; column < columnCount; column++) {

        // Jeśli każdy wiersz ma już swój element główny, kończymy
        if (pivotRow == rowCount) {
            break;
        }

        // 1. Znajdź największą liczbę w tej kolumnie (od pivotRow w dół)
        int bestRow = findPivotRow(matrix, column, pivotRow);
        double bestValue = matrix.get(bestRow, column);

        if (isZero(bestValue)) {
            // W tej kolumnie są same zera, więc nie ma nic do zrobienia.
            // Przechodzimy do następnej kolumny. pivotRow się nie zmienia.
        } else {
            // 2. Przenieś ten wiersz na miejsce pivotRow
            swapRows(matrix, pivotRow, bestRow);

            // 3. Podziel wiersz tak, żeby element główny stał się 1
            double pivotValue = matrix.get(pivotRow, column);
            divideRow(matrix, pivotRow, pivotValue);

            // 4. Wyzeruj każdą INNĄ liczbę w tej kolumnie
            for (int row = 0; row < rowCount; row++) {
                if (row != pivotRow) {
                    double factor = matrix.get(row, column);
                    subtractRowMultiple(matrix, row, pivotRow, factor);
                }
            }

            // Następny element główny trafi o jeden wiersz niżej
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
    Matrix joined = joinSideBySide(original, identity);   // [A | I]
    Matrix reduced = rref(joined);

    // Jeśli lewa część nie stała się I, to A nie ma odwrotności
    if (!leftPartIsIdentity(reduced, size)) {
        throw MatrixError("Macierz jest osobliwa, nie ma odwrotności");
    }

    // Prawa część to wynik
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

    Matrix joined = joinSideBySide(a, b);    // [A | B]
    Matrix reduced = rref(joined);

    if (!leftPartIsIdentity(reduced, size)) {
        throw MatrixError("Brak jednoznacznego rozwiązania");
    }

    // Prawa część to rozwiązanie X
    return copyColumns(reduced, size, b.getColumns());
}

Matrix power(const Matrix& original, int exponent) {
    if (original.getRows() != original.getColumns()) {
        throw MatrixError("Potęgowanie wymaga macierzy kwadratowej");
    }

    // Ujemny wykładnik: najpierw odwracamy, potem używamy dodatniego wykładnika
    if (exponent < 0) {
        Matrix inverted = inv(original);
        return power(inverted, -exponent);
    }

    // Zaczynamy od I, potem mnożymy przez A "exponent" razy
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
