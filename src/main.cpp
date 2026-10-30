#include <iostream>
#include "core/Matrix.hpp"
#include "core/MatrixError.hpp"

int main() {
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

    return 0;
}