#pragma once

#include "Matrix.hpp"

Matrix rref(const Matrix& original);
int rank(const Matrix& original);
Matrix inv(const Matrix& original);
Matrix solve(const Matrix& a, const Matrix& b);
Matrix power(const Matrix& original, int exponent);
double det(const Matrix& original);
double trace(const Matrix& original);
