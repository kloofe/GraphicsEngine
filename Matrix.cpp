#include <vector>
#include <cmath>
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

Matrix::Matrix(std::vector<double> v) 
    : rowNum{1}, colNum{(unsigned int) v.size()} {
    std::vector<double> row;
    for(unsigned int i =  0; i < v.size(); i++) {
        row.push_back(v[i]);
    }
    mtx.push_back(row);
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

Matrix Matrix::normalize() {
    Matrix m{rowNum, colNum};
    double length = 0;
    for(int i = 0; i < colNum; i++) {
        length += getValue(0, i) * getValue(0, i);
    }
    length = std::sqrt(length);
    for(int i = 0; i < colNum; i++) {
        m.setValue(0, i, getValue(0, i)/length);
    }
    return m;
}

std::vector<double> Matrix::toVector() {
    std::vector<double> v;
    for(int i = 0; i < rowNum; i++) {
        for(int j = 0; j < colNum; j++) {
            v.push_back(getValue(i, j));
        }
    }
    return v;
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
            for(unsigned int j2 = 0; j2 < matr.getRows(); j2++) {
                sum += getValue(i, j2) * matr.getValue(j2, j);
            }
            newMtx.setValue(i, j, sum);
        }
    }

    return newMtx;
}

Matrix Matrix::crossProduct(Matrix m) {
    Matrix temp{2, 3};
    for(int i = 0; i < colNum; i++) {
        temp.setValue(0, i, getValue(0, i));
        temp.setValue(1, i, m.getValue(0, i));
        std::cout << "ahhh " << temp.getValue(0, i) << " " << temp.getValue(1, i) << std::endl;
    }
    Matrix result{1, 3};
    result.setValue(0, 0, temp.getValue(0, 1) * temp.getValue(1, 2) - temp.getValue(0, 2) * temp.getValue(1, 1));
    result.setValue(0, 1, -(temp.getValue(0, 0) * temp.getValue(1, 2) - temp.getValue(0, 2) * temp.getValue(1, 0)));
    result.setValue(0, 2, temp.getValue(0, 0) * temp.getValue(1, 1) - temp.getValue(0, 1) * temp.getValue(1, 0));
    std::cout << "CROSS PRODUCT ";
    for(int i = 0; i < 3; i++) {
        std::cout << result.getValue(0, i) << " ";
    }
    std::cout << std::endl;
    return result;

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

