#include "Tokenizer.hpp"
#include <cctype>
#include <stdexcept>
#include <algorithm>

std::vector<Token> tokenize(const std::string& input) {
    int index = 0;
    while (index < input.size()) {
        if (input[index] != kindName())
            throw ParseError("Nieznany token ", index)
        if (std::isspace(input[index])) index++;
        
    }
}