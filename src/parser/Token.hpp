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
