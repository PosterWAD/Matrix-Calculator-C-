#include "ConsoleMenu.hpp"
#include "../core/MatrixError.hpp"
#include "../parser/ParseError.hpp"
#include "../parser/Parser.hpp"
#include "../parser/Tokenizer.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

// ======================= POMOCNICZE =======================

// Wypisuje pytanie i czyta całą linię. Gdy wejście się skończyło (Ctrl+Z), kończy program.
std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cout << "\n";
        std::exit(0);
    }
    return line;
}

// Pyta o liczbę całkowitą z zakresu min..max, aż użytkownik poda poprawną.
int readInt(const std::string& prompt, int min, int max) {
    while (true) {
        std::istringstream stream(readLine(prompt));
        int number;
        std::string rest;
        // ">> rest" sprawdza, czy po liczbie nie ma śmieci, np. "2abc"
        if (stream >> number && !(stream >> rest) && number >= min && number <= max) {
            return number;
        }
        std::cout << "Podaj liczbę całkowitą od " << min << " do " << max << ".\n";
    }
}

// Zamienia macierz na tekst:  "1 2\n3 4\n"
std::string matrixToText(const Matrix& matrix) {
    std::ostringstream text;
    for (int r = 0; r < matrix.getRows(); r++) {
        for (int c = 0; c < matrix.getColumns(); c++) {
            text << "  " << matrix.get(r, c);
        }
        text << "\n";
    }
    return text.str();
}

// Zamienia wynik (liczbę albo macierz) na tekst.
std::string valueToText(const Value& value) {
    if (std::holds_alternative<double>(value)) {
        std::ostringstream text;
        text << "  " << std::get<double>(value) << "\n";
        return text.str();
    }
    return matrixToText(std::get<Matrix>(value));
}

// Macierz startowa 2x2 z liczbami, żeby od razu było co liczyć.
Matrix makeMatrix(double v00, double v01, double v10, double v11) {
    Matrix matrix(2, 2);
    matrix.set(0, 0, v00);
    matrix.set(0, 1, v01);
    matrix.set(1, 0, v10);
    matrix.set(1, 1, v11);
    return matrix;
}

// ======================= MENU =======================

ConsoleMenu::ConsoleMenu()
    : a(makeMatrix(1, 2, 3, 4)), b(makeMatrix(5, 6, 7, 8)) {}

void ConsoleMenu::run() {
    while (true) {
        showScreen();
        int choice = readInt("Wybór: ", 0, 3);
        if (choice == 0) return;
        if (choice == 1) editMatrix(a, "A");
        if (choice == 2) editMatrix(b, "B");
        if (choice == 3) calculate();
    }
}

void ConsoleMenu::showScreen() const {
    std::cout << "\n========== KALKULATOR MACIERZY ==========\n";
    std::cout << "A (" << a.getRows() << "x" << a.getColumns() << "):\n" << matrixToText(a);
    std::cout << "B (" << b.getRows() << "x" << b.getColumns() << "):\n" << matrixToText(b);
    if (!lastResult.empty()) {
        std::cout << "-----------------------------------------\n" << lastResult;
    }
    std::cout << "-----------------------------------------\n";
    std::cout << "1. Wpisz macierz A\n";
    std::cout << "2. Wpisz macierz B\n";
    std::cout << "3. Oblicz wyrażenie\n";
    std::cout << "0. Wyjście\n";
}

// Pyta o rozmiar i wiersze. Nowa macierz powstaje "na boku" -
// stara zostaje podmieniona dopiero, gdy WSZYSTKIE wiersze są poprawne.
void ConsoleMenu::editMatrix(Matrix& matrix, const std::string& name) {
    int rows = readInt("Liczba wierszy " + name + " (1-8): ", 1, 8);
    int columns = readInt("Liczba kolumn " + name + " (1-8): ", 1, 8);
    Matrix edited(rows, columns);

    std::cout << "Wpisuj wiersze, liczby oddzielone spacją (np. 1 2.5 -3).\n";
    for (int r = 0; r < rows; r++) {
        std::istringstream stream(readLine("Wiersz " + std::to_string(r + 1) + ": "));
        std::vector<double> numbers;
        double number;
        while (stream >> number) {
            numbers.push_back(number);
        }

        bool onlyNumbers = stream.eof();  // nie zatrzymało się na czymś, co nie jest liczbą
        if (!onlyNumbers || static_cast<int>(numbers.size()) != columns) {
            lastResult = "Błąd: wiersz " + std::to_string(r + 1) + " musi mieć dokładnie " +
                         std::to_string(columns) + " liczb(y). Macierz " + name + " bez zmian.\n";
            return;  // "matrix" nie zostało ruszone
        }
        for (int c = 0; c < columns; c++) {
            edited.set(r, c, numbers[c]);
        }
    }

    matrix = edited;
    lastResult = "Zapisano macierz " + name + ".\n";
}

void ConsoleMenu::calculate() {
    std::cout << "Działania: + - * ^ ^T ( )   funkcje: inv det trans rref tr rank solve(A, B)\n";
    std::string expression = readLine("Wyrażenie: ");

    try {
        Parser parser(tokenize(expression), a, b);
        lastResult = "Wynik " + expression + ":\n" + valueToText(parser.parse());
    } catch (const ParseError& error) {
        lastResult = "Błąd składni: " + std::string(error.what()) + "\n";
    } catch (const MatrixError& error) {
        lastResult = "Błąd obliczeń: " + std::string(error.what()) + "\n";
    }
}
