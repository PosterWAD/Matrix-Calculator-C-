#pragma once

#include "../core/Matrix.hpp"
#include "../parser/Parser.hpp"
#include <string>

// Wszystko, co widać w oknie: edytory macierzy A i B, pole wyrażenia i wynik.
// draw() jest wołane w każdej klatce (ok. 60 razy na sekundę) i rysuje okno od nowa.
class CalculatorPanel {
public:
    CalculatorPanel();
    void draw();

private:
    Matrix a;
    Matrix b;
    char expression[256];    // tekst w polu wyrażenia (ImGui potrzebuje zwykłej tablicy char)
    bool hasResult = false;  // czy policzyliśmy już cokolwiek
    Value result;            // ostatni wynik (liczba albo macierz)
    std::string errorText;   // ostatni błąd (pusty = brak błędu)

    void drawMatrixEditor(const char* name, Matrix& matrix);
    void drawExpressionInput();
    void drawResult();
    void drawHelp();
    void appendToExpression(const char* text);
    void calculate();
};
