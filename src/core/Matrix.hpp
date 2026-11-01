#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

class Matrix {
    private:
    int rows;
    int columns;
    std::vector<std::vector<double>> values;

    public:
    void set(int row, int column, double value);
    void print() const;
    Matrix(int rows, int columns);
    void userInputValues();
    Matrix add(const Matrix& other) const;
    Matrix subtract(const Matrix& other) const;
    Matrix multiply(const Matrix& other) const;
    Matrix divide(const Matrix& other) const;
    Matrix transpose() const;
    Matrix inverse() const;
    Matrix identity() const;
    double determinant() const;
    double trace() const;

};

#endif