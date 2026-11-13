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
    double get(int row, int column) const;
    int getRows() const { return rows; }
    int getColumns() const { return columns; }
    void print() const;
    Matrix(int rows, int columns);
    void userInputValues();
    Matrix add(const Matrix& other) const;
    Matrix subtract(const Matrix& other) const;
    Matrix multiply(const Matrix& other) const;
    Matrix divide(const Matrix& other) const;
    Matrix scale(double factor) const;
    Matrix transpose() const;
    Matrix inverse() const;
    static Matrix identity(int n);
    double determinant() const;
    double trace() const;

};

#endif