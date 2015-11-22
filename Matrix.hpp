#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>

class Matrix
{
	public:
		Matrix();
        Matrix(unsigned int m, unsigned int n);
        Matrix(const std::vector<double>& vect);
		//Matrix(const Matrix& matr);
		//~Matrix();
		//Matrix operator=(const Matrix& matr);

		//Matrix operator+(const double& scal);
		Matrix operator*(const double& scal);
		Matrix operator+(const std::vector<double>& vect);
		Matrix operator*(const std::vector<double>& vect);
		Matrix operator+(const Matrix& matr);
		Matrix operator*(const Matrix& matr);

        void setValue(unsigned int m, unsigned int n, double value);
        double getValue(unsigned int mIndex, unsigned int nIndex) const;
        unsigned int getRows() const;
        unsigned int getCols() const;

	private:
		std::vector<std::vector<double> > mtx;
        unsigned int rowNum;
        unsigned int colNum;

        void initMatrix();
};


#endif // MATTRIX_HPP
