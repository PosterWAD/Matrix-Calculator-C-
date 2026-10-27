#ifndef MATRIX_HPP
#define MATRIX_HPP

class Matrix {
    private:
    int rows;
    int columns;
    double values[8][8];

    public:
    Matrix(int rows, int columns);
    Matrix userInputValues();
    Matrix add(Matrix& other);
    Matrix subtract(Matrix& other);
    Matrix multiply(Matrix& other);
    // Matrix divide(Matrix& other);    //do dodania później jako mnożenie przez odwrotność
    Matrix transpose();
    Matrix inverse();
    double determinant();
    double trace();

};

#endif