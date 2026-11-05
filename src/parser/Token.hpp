#pragma once
#include <string>

enum class TokenKind {
    Number,         /* 2  2.5  1e-3 */
    MatrixName,     /* A  B  a  b (only A and B) */
    Plus,           /* +  in  A + B */
    BinaryMinus,    /* -  in  A - B,  3 - 2,  (A) - B */
    UnaryMinus,     /* -  in  -A,  3 * -B,  (-A) */
    Star,           /* *  in  2 * A,  A * B */
    Power,          /* ^  in  A^2,  2^3 (the number after it is a separate token) */
    Transpose,      /* ^T  in  A^T,  inv(A)^T (one token, not ^ + T) */
    Function,       /* inv  det  trans  rref  tr  rank  solve */
    LeftParen,      /* (  in  inv(A),  (A + B) */
    RightParen,     /* )  in  inv(A),  (A + B) */
    Comma           /* ,  in  solve(A, B) */
};

struct Token {
    TokenKind kind = TokenKind::Number; // default value like the rest down below to prevent future bugs
    std::string text;       // "inv", "A", "2.5", "^T" ...
    int position = 0;       // index in the input string
    double value = 0.0;     // used only when kind == Number
};

inline const char* kindName(TokenKind kind) {
    switch (kind) {
        case TokenKind::Number:         return "Number";
        case TokenKind::MatrixName:     return "MatrixName";
        case TokenKind::Plus:           return "Plus";
        case TokenKind::BinaryMinus:    return "BinaryMinus";
        case TokenKind::UnaryMinus:     return "UnaryMinus";
        case TokenKind::Star:           return "Star";
        case TokenKind::Power:          return "Power";
        case TokenKind::Transpose:      return "Transpose";
        case TokenKind::Function:       return "Function";
        case TokenKind::LeftParen:      return "LeftParen";
        case TokenKind::RightParen:     return "RightParen";
        case TokenKind::Comma:          return "Comma";
    }
    return "?";
}
