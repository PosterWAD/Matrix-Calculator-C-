    #include "Parser.hpp"
#include "ParseError.hpp"
#include "../core/MatrixError.hpp"
#include "../core/Operations.hpp"
#include <cmath>

// Value to liczba albo macierz, więc każda operacja sprawdza, co dostała.

bool isNumber(const Value& value) {
    return std::holds_alternative<double>(value);
}

Matrix toMatrix(const Value& value) {
    if (isNumber(value)) {
        throw MatrixError("Ta operacja wymaga macierzy, a nie liczby");
    }
    return std::get<Matrix>(value);
}

Value negateValue(const Value& x) {
    if (isNumber(x)) return -std::get<double>(x);
    return std::get<Matrix>(x).scale(-1);
}

Value addValues(const Value& x, const Value& y) {
    if (isNumber(x) && isNumber(y)) return std::get<double>(x) + std::get<double>(y);
    if (isNumber(x) || isNumber(y)) throw MatrixError("Nie można dodać liczby do macierzy");
    return std::get<Matrix>(x).add(std::get<Matrix>(y));
}

Value multiplyValues(const Value& x, const Value& y) {
    if (isNumber(x) && isNumber(y)) return std::get<double>(x) * std::get<double>(y);
    if (isNumber(x)) return std::get<Matrix>(y).scale(std::get<double>(x));  // 2 * A
    if (isNumber(y)) return std::get<Matrix>(x).scale(std::get<double>(y));  // A * 2
    return std::get<Matrix>(x).multiply(std::get<Matrix>(y));                // A * B
}

Value powerValues(const Value& base, const Value& exponent) {
    if (!isNumber(exponent)) throw MatrixError("Wykładnik musi być liczbą");
    double e = std::get<double>(exponent);
    if (isNumber(base)) return std::pow(std::get<double>(base), e);
    if (e != std::floor(e)) throw MatrixError("Wykładnik potęgi macierzy musi być całkowity");
    return power(std::get<Matrix>(base), static_cast<int>(e));  // A^-1 = inv(A)
}

Value applyFunction(const Token& token, const std::vector<Value>& args) {
    const std::string& name = token.text;
    size_t expected = (name == "solve") ? 2 : 1;
    if (args.size() != expected) {
        throw ParseError("Zła liczba argumentów funkcji " + name, token.position);
    }

    if (name == "inv")   return inv(toMatrix(args[0]));
    if (name == "det")   return det(toMatrix(args[0]));
    if (name == "trans") return toMatrix(args[0]).transpose();
    if (name == "rref")  return rref(toMatrix(args[0]));
    if (name == "tr")    return trace(toMatrix(args[0]));
    if (name == "rank")  return static_cast<double>(rank(toMatrix(args[0])));
    return solve(toMatrix(args[0]), toMatrix(args[1]));
}

// ======================= PARSER =======================
// Gramatyka: każda reguła to jedna metoda. Im NIŻEJ reguła, tym WIĘKSZA waga
// (tym wcześniej się liczy), bo wyższa reguła woła niższą po swoje argumenty.
//
//   expression = term    { (+|-) term }       A + B - A
//   term       = unary   { * unary }          2 * A
//   unary      = - unary | powerExpr          -A,  - -A
//   powerExpr  = postfix [ ^ unary ]          A^2,  A^-1
//   postfix    = primary { ^T }               A^T
//   primary    = liczba | macierz | funkcja(arg, ...) | ( expression )

Parser::Parser(const std::vector<Token>& tokens, const Matrix& a, const Matrix& b)
    : tokens(tokens), a(a), b(b) {}

Value Parser::parse() {
    if (tokens.empty()) {
        throw ParseError("Puste wyrażenie", 0);
    }
    Value result = expression();
    if (index < tokens.size()) {  // "A B" - po całym wyrażeniu coś zostało
        throw ParseError("Brakuje operatora", currentPosition());
    }
    return result;
}

Value Parser::expression() {
    Value result = term();
    while (true) {
        if (match(TokenKind::Plus))       result = addValues(result, term());
        else if (match(TokenKind::Minus)) result = addValues(result, negateValue(term()));
        else return result;
    }
}

Value Parser::term() {
    Value result = unary();
    while (match(TokenKind::Star)) {
        result = multiplyValues(result, unary());
    }
    return result;
}

Value Parser::unary() {
    if (match(TokenKind::Minus)) {
        return negateValue(unary());
    }
    return powerExpr();
}

Value Parser::powerExpr() {
    Value base = postfix();
    if (match(TokenKind::Power)) {
        // wykładnik to "unary", więc działa A^-1 i 2^3^2 = 2^(3^2)
        return powerValues(base, unary());
    }
    return base;
}

Value Parser::postfix() {
    Value result = primary();
    while (match(TokenKind::Transpose)) {
        result = toMatrix(result).transpose();
    }
    return result;
}

Value Parser::primary() {
    if (index >= tokens.size()) {
        throw ParseError("Niedokończone wyrażenie", currentPosition());
    }
    Token token = tokens[index++];

    switch (token.kind) {
        case TokenKind::Number:
            return token.value;

        case TokenKind::MatrixName:
            if (token.text == "a") return a;
            return b;

        case TokenKind::LeftParen: {
            Value inside = expression();
            expect(TokenKind::RightParen, "Brakuje nawiasu )");
            return inside;
        }

        case TokenKind::Function: {
            expect(TokenKind::LeftParen, "Po funkcji musi być nawias (");
            std::vector<Value> args;
            args.push_back(expression());
            while (match(TokenKind::Comma)) {  // solve(A, B)
                args.push_back(expression());
            }
            expect(TokenKind::RightParen, "Brakuje nawiasu )");
            return applyFunction(token, args);
        }

        default:
            throw ParseError("Nieoczekiwany znak: " + token.text, token.position);
    }
}

// ======================= POMOCNICZE =======================

// Jeśli bieżący token jest tego rodzaju, zjedz go i zwróć true.
bool Parser::match(TokenKind kind) {
    if (index < tokens.size() && tokens[index].kind == kind) {
        index++;
        return true;
    }
    return false;
}

// Tak jak match
void Parser::expect(TokenKind kind, const std::string& message) {
    if (!match(kind)) {
        throw ParseError(message, currentPosition());
    }
}

// Pozycja bieżącego tokenu w tekście (albo koniec tekstu).
int Parser::currentPosition() const {
    if (index < tokens.size()) {
        return tokens[index].position;
    }
    const Token& last = tokens.back();
    return last.position + static_cast<int>(last.text.size());
}
