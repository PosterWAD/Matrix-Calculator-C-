#pragma once
#include <stdexcept>
#include <string>

class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message, int position)
        : std::runtime_error(message + " (pozycja " + std::to_string(position) + ")"),
          position(position) {}
    int position;
};