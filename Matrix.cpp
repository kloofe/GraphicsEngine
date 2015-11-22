#include <vector>
#include <numeric>
#include <stdexcept>
#include "Matrix.hpp"
#include <iostream>

Matrix::Matrix() 
    : rowNum{4}, colNum(4) {
    initMatrix();
}

Matrix::Matrix(unsigned int m, unsigned int n)
    : rowNum{m}, colNum{n} {
    initMatrix();
}

Matrix::Matrix(double x, double y, double z, double w) 
    : rowNum{4}, colNum{1} {
    std::vector<double> temp;
    temp.push_back(x);
    temp.push_back(y);
    temp.push_back(z);
    temp.push_back(w);
    for(unsigned int i = 0; i < rowNum; i++) {
        std::vector<double> row;
        row.push_back(temp.at(i));
        mtx.push_back(row);
    }
}

Matrix Matrix::operator*(const double& scal) {
    Matrix newMtx(rowNum, colNum);
    for(unsigned int i = 0; i < rowNum; i++) {
        for(unsigned int j = 0; j < colNum; j++) {
            newMtx.setValue(i, j, getValue(i, j) * scal);
        }
    }
    return newMtx;
}

Matrix Matrix::operator+(const std::vector<double>& vect) {
    if(vect.size() != colNum) {
        throw std::invalid_argument("Incorrect vector size");
    }

    Matrix newMtx(rowNum, colNum);

    for(unsigned int i = 0; i < colNum; i++) {
        newMtx.setValue(0, i, getValue(0, i) + vect.at(i)); 
    }

    return newMtx;
}

Matrix Matrix::operator*(const std::vector<double>& vect) {
    if(colNum != 1) {
        throw std::invalid_argument("Incorrect vector size");
    }
    Matrix newMtx(rowNum, vect.size());

    for(int i = 0; i < rowNum; i++) {
        for(int j = 0; j < vect.size(); j++) {
            newMtx.setValue(i, j, getValue(i, 0) * vect.at(j));
        }
    }

    return newMtx;

}

Matrix Matrix::operator+(const Matrix& matr) {
    if(matr.getRows() != rowNum || matr.getCols() != colNum) {
        throw std::invalid_argument("Incorrect matrix size");
    }

    Matrix newMtx(rowNum, colNum);

    for(unsigned int i = 0; i < rowNum; i++) {
        for(unsigned int j = 0; j < colNum; j++) {
            newMtx.setValue(i, j, getValue(i, j) + matr.getValue(i, j));
        }
    }

    return newMtx;
}

Matrix Matrix::operator*(const Matrix& matr) {
    if(colNum != matr.getRows()) {
        throw std::invalid_argument("Incorrect matrix size");
    }

    Matrix newMtx(rowNum, matr.getCols());

    for(unsigned int i = 0; i < rowNum; i++) {
        for(unsigned int j = 0; j < matr.getCols(); j++) {
            double sum = 0;
            for(unsigned int j2 = 0; j < matr.getCols(); j2++) {
                sum += getValue(j2, i) * matr.getValue(i, j2);
            }
            newMtx.setValue(i, j, sum);
        }
    }

    return newMtx;
}

unsigned int Matrix::getRows() const {
    return rowNum;
}

unsigned int Matrix::getCols() const {
    return colNum;
}

double Matrix::getValue(unsigned int mIndex, unsigned int nIndex) const {
    return mtx.at(mIndex).at(nIndex);
}

void Matrix::setValue(unsigned int m, unsigned int n, double value) {
    mtx.at(m).at(n) = value;
}

void Matrix::initMatrix() {
    for(unsigned int i = 0; i < rowNum; i++) {
        std::vector<double> row;
        for(unsigned int j = 0; j < colNum; j++) {
            row.push_back(0);
        }
        mtx.push_back(row);
    }
}

