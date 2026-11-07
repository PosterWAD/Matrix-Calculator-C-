#include "Tokenizer.hpp"
#include "ParseError.hpp"
#include <cctype>
#include <stdexcept>
#include <string>
#include <vector>

void addToken(std::vector<Token>& tokens, TokenKind kind, const std::string& text, int position) {
    Token token;
    token.kind = kind;
    token.text = text;
    token.position = position;
    tokens.push_back(token);
}

bool isDigitAt(const std::string& input, int index) {
    int length = input.size();
    if (index >= length) {
        return false;
    }
    unsigned char character = input[index];
    return std::isdigit(character) != 0;
}

bool isLetterAt(const std::string& input, int index) {
    int length = input.size();
    if (index >= length) {
        return false;
    }
    unsigned char character = input[index];
    return std::isalpha(character) != 0;
}

bool isFunctionName(const std::string& word) {
    if (word == "inv" || word == "det" || word == "trans" || word == "rref" ||
        word == "tr" || word == "rank" || word == "solve") {
        return true;
    }
    return false;
}

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    int length = input.size();
    int index = 0;

    while (index < length) {
        unsigned char current = input[index];

        if (std::isspace(current)) {
            index++;
        }

        else if (std::isdigit(current)) {
            int start = index;
            std::string text;
            int dotCount = 0;

            while (isDigitAt(input, index) || (index < length && input[index] == '.')) {
                if (input[index] == '.') {
                    if (dotCount == 1) {
                        break;
                    }
                    dotCount++;
                }
                text += input[index];
                index++;
            }

            double value = 0.0;
            try {
                value = std::stod(text);
            } catch (const std::out_of_range&) {
                throw ParseError("Liczba jest za duża", start);
            }

            Token token;
            token.kind = TokenKind::Number;
            token.text = text;
            token.position = start;
            token.value = value;
            tokens.push_back(token);
        }

        else if (std::isalpha(current)) {
            int start = index;
            std::string text; 
            std::string lower;
            while (isLetterAt(input, index)) {
                unsigned char character = input[index];
                text += input[index];
                lower += static_cast<char>(std::tolower(character));
                index++;
            }

            TokenKind kind = TokenKind::MatrixName;
            if (lower == "a" || lower == "b") {
                kind = TokenKind::MatrixName;
            }
            else if (isFunctionName(lower)) {
                kind = TokenKind::Function;
            }
            else {
                throw ParseError("Nieznana macierz lub funkcja: " + text, start);
            }

            addToken(tokens, kind, text, start);
        }

        else if (current == '^') {
            bool nextIsT = (index + 1 < length) &&
                           (input[index + 1] == 'T' || input[index + 1] == 't');
            bool somethingGluedAfterT = isLetterAt(input, index + 2) ||
                                        isDigitAt(input, index + 2);

            if (nextIsT && !somethingGluedAfterT) {
                addToken(tokens, TokenKind::Transpose, "^T", index);
                index += 2;
            }
            else {
                addToken(tokens, TokenKind::Power, "^", index);
                index += 1;
            }
        }

        else {
            switch (current) {
                case '+': addToken(tokens, TokenKind::Plus, "+", index++); break;
                case '-': addToken(tokens, TokenKind::Minus, "-", index++); break;
                case '*': addToken(tokens, TokenKind::Star, "*", index++); break;
                case '(': addToken(tokens, TokenKind::LeftParen, "(", index++); break;
                case ')': addToken(tokens, TokenKind::RightParen, ")", index++); break;
                case ',': addToken(tokens, TokenKind::Comma, ",", index++); break;
                default:
                    throw ParseError("Nieznany znak", index);
            }
    }

    return tokens;
}
