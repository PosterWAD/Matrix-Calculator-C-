#pragma once

#include "../core/Matrix.hpp"
#include <string>

// Menu konsolowe: trzyma macierze A i B oraz ostatni wynik.
// To jedyne miejsce w programie, które czyta klawiaturę.
class ConsoleMenu {
public:
    ConsoleMenu();
    void run();  // pętla menu, kończy się po wybraniu 0

private:
    Matrix a;
    Matrix b;
    std::string lastResult;  // tekst ostatniego wyniku albo błędu

    void showScreen() const;
    void editMatrix(Matrix& matrix, const std::string& name);
    void calculate();
};
