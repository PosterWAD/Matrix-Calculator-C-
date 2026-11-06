#include "Tokenizer.hpp"
#include "ParseError.hpp"
#include <cctype>

std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    int length = input.size();
    int index = 0;

    while (index < length) {
        unsigned char current = input[index];

        if (std::isspace(current)) {
            index++;                    // spacja: pomijamy i idziemy dalej
        }
        else if (std::isdigit(current)) {
            // TODO: liczba. Czytaj znaki, dopóki są cyframi albo kropką.
            // Zapamiętaj, gdzie liczba się zaczęła (do pola position).
        }
        else if (std::isalpha(current)) {
            // TODO: słowo. Czytaj znaki, dopóki są literami.
            // Potem sprawdź: "a"/"b" → MatrixName, "inv"/"det"/... → Function,
            // cokolwiek innego → throw ParseError
        }
        else if (current == '^') {
            // TODO: jeśli następny znak to 'T' albo 't' → Transpose (zjada 2 znaki)
            //       w przeciwnym razie → Power (zjada 1 znak)
        }
        else if (current == '+') {
            Token token;
            token.kind = TokenKind::Plus;
            token.text = "+";
            token.position = index;
            tokens.push_back(token);
            index++;
        }
        else if (current == '-') {
            Token token;
            token.kind = TokenKind::Minus;
            token.text = "-";
            token.position = index;
            tokens.push_back(token);
            index++;
        }
        else if (current == '*') {
            Token token;
            token.kind = TokenKind::Star;
            token.text = "*";
            token.position = index;
            tokens.push_back(token);
            index++;
        }
        else if (current == '(') {
            Token token;
            token.kind = TokenKind::LeftParen;
            token.text = "(";
            token.position = index;
            tokens.push_back(token);
            index++;
        }
        else if (current == ')') {
            Token token;
            token.kind = TokenKind::RightParen;
            token.text = ")";
            token.position = index;
            tokens.push_back(token);
            index++;
        }
        else if (current == ',') {
            Token token;
            token.kind = TokenKind::Comma;
            token.text = ",";
            token.position = index;
            tokens.push_back(token);
            index++;
        } 
        else {
            throw ParseError("Nieznany znak", index);
        }
    }

    return tokens;
}
