#pragma once

#include "Token.hpp"
#include "../core/Matrix.hpp"
#include <string>
#include <variant>
#include <vector>

using Value = std::variant<double, Matrix>; // Wynik: albo liczba (np. det(A)), albo macierz (np. A + B).

class Parser {
public:
    Parser(const std::vector<Token>& tokens, const Matrix& a, const Matrix& b);
    Value parse();

private:
    std::vector<Token> tokens;
    Matrix a;
    Matrix b;
    size_t index = 0;

    Value expression();  // +  -
    Value term();        // *
    Value unary();       // -A
    Value powerExpr();   // ^
    Value postfix();     // ^T
    Value primary();     // liczba, macierz, funkcja

    bool match(TokenKind kind);
    void expect(TokenKind kind, const std::string& message);
    int currentPosition() const;
};
