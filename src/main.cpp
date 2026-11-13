#include <iostream>
#include "core/Matrix.hpp"
#include "core/MatrixError.hpp"
#include "core/Operations.hpp"
#include "parser/Parser.hpp"
#include "parser/ParseError.hpp"
#include "parser/Tokenizer.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

// Wypisuje wynik: liczbę albo macierz.
void printValue(const Value& value) {
    if (std::holds_alternative<double>(value)) {
        std::cout << std::get<double>(value) << "\n";
    } else {
        std::get<Matrix>(value).print();
    }
}

// Tokenizuje, parsuje i wypisuje wynik albo błąd.
void testExpression(const std::string& expression, const Matrix& a, const Matrix& b) {
    std::cout << ">>> " << expression << "\n";
    try {
        std::vector<Token> tokens = tokenize(expression);
        Parser parser(tokens, a, b);
        Value result = parser.parse();
        printValue(result);
    } catch (const ParseError& error) {
        std::cout << "Błąd składni: " << error.what() << "\n";
    } catch (const MatrixError& error) {
        std::cout << "Błąd obliczeń: " << error.what() << "\n";
    }
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    Matrix a(2, 2);
    a.set(0, 0, 1);
    a.set(0, 1, 2);
    a.set(1, 0, 3);
    a.set(1, 1, 4);

    Matrix b(2, 2);
    b.set(0, 0, 5);
    b.set(0, 1, 6);
    b.set(1, 0, 7);
    b.set(1, 1, 8);

    try {
        Matrix result = a.add(b);
        result.print();
    } catch (const MatrixError& error) {
        std::cout << error.what() << "\n";
    }

    // ===== Testy fazy 2 (w komentarzu: oczekiwany wynik) =====
    try {
        std::cout << "--- rref(a) ---\n";
        rref(a).print();                                // 1 0 / 0 1

        std::cout << "rank(a) = " << rank(a) << "\n";   // 2
        std::cout << "det(a) = " << det(a) << "\n";     // -2
        std::cout << "trace(a) = " << trace(a) << "\n"; // 5

        std::cout << "--- inv(a) ---\n";
        inv(a).print();                                 // -2 1 / 1.5 -0.5

        std::cout << "--- solve ---\n";
        Matrix rhs(2, 1);
        rhs.set(0, 0, 5);
        rhs.set(1, 0, 11);
        solve(a, rhs).print();                          // 1 / 2

        std::cout << "--- power ---\n";
        Matrix diag(2, 2);
        diag.set(0, 0, 2);
        diag.set(1, 1, 3);
        power(diag, 2).print();                         // 4 0 / 0 9

        std::cout << "--- macierz osobliwa ---\n";
        Matrix singular(2, 2);
        singular.set(0, 0, 1);
        singular.set(0, 1, 2);
        singular.set(1, 0, 2);
        singular.set(1, 1, 4);
        std::cout << "rank = " << rank(singular) << "\n";   // 1
        std::cout << "det = " << det(singular) << "\n";     // 0
        try {
            inv(singular);
        } catch (const MatrixError& error) {
            std::cout << error.what() << "\n";              // Macierz jest osobliwa...
        }
        try {
            solve(singular, rhs);
        } catch (const MatrixError& error) {
            std::cout << error.what() << "\n";              // Brak jednoznacznego rozwiązania
        }
    } catch (const MatrixError& error) {
        std::cout << "NIEOCZEKIWANY BŁĄD: " << error.what() << "\n";
    }

    // ===== Testy fazy 4: parser (w komentarzu: oczekiwany wynik) =====
    std::cout << "\n===== PARSER =====\n";
    testExpression("2 + 3 * 4", a, b);          // 14
    testExpression("2 * A + B", a, b);          // 7 10 / 13 16
    testExpression("A * (A + B)", a, b);        // 26 32 / 58 72
    testExpression("A - B - A", a, b);          // -5 -6 / -7 -8
    testExpression("-A", a, b);                 // -1 -2 / -3 -4
    testExpression("2 * A + trans(B)", a, b);   // 7 11 / 12 16
    testExpression("A^T", a, b);                // 1 3 / 2 4
    testExpression("A^2", a, b);                // 7 10 / 15 22
    testExpression("A^-1", a, b);               // -2 1 / 1.5 -0.5
    testExpression("det(A)", a, b);             // -2
    testExpression("INV(a)", a, b);             // -2 1 / 1.5 -0.5
    testExpression("rank(A) + tr(A)", a, b);    // 7
    testExpression("solve(A, B)", a, b);        // -3 -4 / 4 5
    testExpression("A + 1", a, b);              // błąd obliczeń
    testExpression("(A + B", a, b);             // błąd składni
    testExpression("A B", a, b);                // błąd składni
    testExpression("inv(2)", a, b);             // błąd obliczeń
    testExpression("", a, b);                   // błąd składni

    return 0;
}