#include <iostream>
#include "core/Matrix.hpp"
#include "core/MatrixError.hpp"
#include "core/Operations.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

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

    return 0;
}