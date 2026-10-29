#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

class Matrix {
    private:
    int rows;
    int columns;
    std::vector<std::vector<double>> values;

    public:
    Matrix(int rows, int columns);
    void userInputValues();
    Matrix add(Matrix& other);
    Matrix subtract(Matrix& other);
    Matrix multiply(Matrix& other);
    // Matrix divide(Matrix& other);    //do dodania później jako mnożenie przez odwrotność
    Matrix transpose();
    Matrix inverse();
    Matrix identity();
    double determinant() const;
    double trace();

};

#endif