#pragma once
#include <cmath>
#include <stdexcept>
#include "defs.h"

namespace ifmo_mtp_utils {
template<class T = double> class Matrix {
public:
	virtual size_t nRows() const = 0;
	virtual size_t nCols() const = 0;

	virtual Matrix<T> &operator=(const Matrix<T> &matrix) = 0;
	virtual T &operator()(size_t i) = 0;
	virtual T &operator()(size_t i, size_t j) = 0;
	virtual Matrix<T> &row(size_t i) = 0;
	virtual Matrix<T> &col(size_t i) = 0;

	Matrix<T> &operator+=(const Matrix<T> &matrix) {
		requireSum(*this, matrix);
		size_t nRows = this->nRows();
		size_t nCols = this->nCols();

		Matrix<T> &matr = const_cast<Matrix<T>&>(matrix);
		for (size_t i = 0; i < nRows; i++)
			for (size_t j = 0; j < nCols; j++)
				(*this)(i, j) += matr(i, j);

		return *this;
	}

	Matrix<T> &operator-=(const Matrix<T> &matrix) {
		requireSum(*this, matrix);
		size_t nRows = this->nRows();
		size_t nCols = this->nCols();

		Matrix<T> &matr = const_cast<Matrix<T>&>(matrix);
		for (size_t i = 0; i < nRows; i++)
			for (size_t j = 0; j < nCols; j++)
				(*this)(i, j) -= matr(i, j);

		return *this;
	}

	Matrix<T> &operator-() {
		size_t nRows = this->nRows();
		size_t nCols = this->nCols();

		for (size_t i = 0; i < nRows; i++)
			for (size_t j = 0; j < nCols; j++) {
				T &ref = (*this)(i, j);
				ref = -ref;
			}

		return *this;
	}

	bool operator==(const Matrix<T> &matrix) const {
		size_t nRows = this->nRows();
		size_t nCols = this->nCols();
		if (nRows != matrix.nRows() || nCols != matrix.nCols())
			return false;

		Matrix<T> &thisRef = const_cast<Matrix<T>&>(*this);
		Matrix<T> &matr = const_cast<Matrix<T>&>(matrix);
		for(size_t i = 0; i < nRows; i++)
			for(size_t j = 0; j < nCols; j++)
				if (thisRef(i, j) != matr(i, j))
					return false;

		return true;
	}

	virtual ~Matrix() {}
};

template<class T>
void requireSum(const Matrix<T> &a, const Matrix<T> &b) {
	if (a.nRows() != b.nRows() || a.nCols() != b.nCols())
		throw std::invalid_argument("Operands must have the same dimensions");
}
}

namespace std {
/**
 * Equality of Matrix<double>
 * with defined precision ({@code DBL_PREC})
 */
template<> struct equal_to<ifmo_mtp_utils::Matrix<>> {
	typedef ifmo_mtp_utils::Matrix<> Matrix;
	bool operator()(const Matrix &ca, const Matrix &cb) const {
		size_t nRows = ca.nRows();
		size_t nCols = ca.nCols();
		if (nRows != cb.nRows() || nCols != cb.nCols())
			return false;

		Matrix &a = const_cast<Matrix&>(ca);
		Matrix &b = const_cast<Matrix&>(cb);
		for(size_t i = 0; i < nRows; i++)
			for(size_t j = 0; j < nCols; j++) {
				double diff = a(i, j) - b(i, j);
				if (abs(diff) >= DBL_PREC)
					return false;
			}

		return true;
	}
};
}
