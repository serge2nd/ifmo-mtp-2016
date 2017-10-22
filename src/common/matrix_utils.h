#pragma once
#include <fstream>
#include <iomanip>
#include <string>
#include <stdexcept>
#include "matrix2d.h"
#include "utils.h"

namespace ifmo_mtp_utils {
using std::string;
using std::to_string;
using std::istream;
using std::ostream;
using std::setw;
using std::endl;
using std::streamsize;
using std::invalid_argument;
using std::runtime_error;

// ********************
// *** Input/Output ***
// ********************
template<class T>
void loadMatrix(istream &stream, Matrix<T> &matrix, bool replace = true, size_t nRows = 0, size_t nCols = 0) {
	if (nRows == 0 && !(stream >> nRows))
		throw runtime_error("Reading rows count failed");
	if (nCols == 0 && !(stream >> nCols))
		throw runtime_error("Reading columns count failed");
	
	T val;
	if (replace) {
		matrix = Matrix2D<T>(nRows, nCols);
		Matrix2D<T>* newMatr = dynamic_cast<Matrix2D<T>*>(&matrix);

		for (size_t i = 0; i < nRows; i++) {
			for (size_t j = 0; j < nCols; j++) {
				require(stream >> val, "Reading matrix("
						+ to_string(i) + ", " + to_string(j)
						+ ") failed");
				newMatr->data_[i][j] = val;
			}
		}
	}
	else {
		for (size_t i = 0; i < nRows; i++) {
			for (size_t j = 0; j < nCols; j++) {
				require(stream >> val, "Reading matrix("
						+ to_string(i) + ", " + to_string(j)
						+ ") failed");
				matrix(i, j) = val;
			}
		}
	}
}

template<class T>
ostream &operator<<(ostream &stream, const Matrix<T> &matrix) {
	streamsize cellWidth = stream.width();
	size_t nRows = matrix.nRows(),
		nCols = matrix.nCols();

	Matrix<T> &matr = const_cast<Matrix<T>&>(matrix);
	for (size_t i = 0; i < nRows; i++) {
		for (size_t j = 0; j < nCols; j++) {
			require(stream << setw(cellWidth) << matr(i, j),
				"Writing matrix(" + to_string(i) + ", "
				+ to_string(j) + ") failed");
		}
		require(stream << endl, "Writing matrix row "
				+ to_string(i) + " failed");
	}

	return stream;
}

// ***************
// *** Product ***
// ***************
template<class T>
void requireMultiplying(const Matrix<T> &a, const Matrix<T> &b) {
	if (a.nCols() != b.nRows())
		throw invalid_argument("Matrices " + to_string(a.nRows()) + "x" + to_string(a.nCols())
			+ " and " + to_string(b.nRows()) + "x" + to_string(b.nCols())
			+ " cannot be multiplied");
}

template<class T>
Matrix2D<T> mp(const Matrix<T> &ca, const Matrix<T> &cb) {
	requireMultiplying(ca, cb);
	size_t m = ca.nRows(), n = cb.nCols(), p = ca.nCols();
	Matrix2D<> c(m, n);

	Matrix<T> &a = const_cast<Matrix<T>&>(ca);
	Matrix<T> &b = const_cast<Matrix<T>&>(cb);
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < n; j++) {
			double cell = c(i, j) = 0;
			for (size_t k = 0; k < p; k++) {
				cell += (a(i, k) * b(k, j));
			}
			c(i, j) = cell;
		}
	}

	return c;
}

// ***********************************************************
// *** Derived matrices such as unit matrix, diagonal, ... ***
// ***********************************************************
Matrix2D<> e(size_t n) {
	double zero = 0;
	Matrix2D<> matr(n, n, &zero);
	for (int i = 0; i < n; i++)
		matr.data_[i][i] = 1;
	return matr;
}

template<class T>
Matrix2D<T> t(const Matrix<T> &matrix) {
	size_t nRows = matrix.nRows(),
		nCols = matrix.nCols();
	Matrix<T> &matr = const_cast<Matrix<T>&>(matrix);
	Matrix2D<T> result(nCols, nRows);

	for (size_t i = 0; i < nRows; i++)
		for (size_t j = 0; j < nCols; j++)
			result.data_[j][i] = matr(i, j);

	return result;
}

Matrix2D<> diag(const Matrix<> &matrix) {
	size_t nRows = matrix.nRows(),
		nCols = matrix.nCols();
	if (nRows != nCols)
		throw invalid_argument("diag() requires a square matrix");

	Matrix<> &matr = const_cast<Matrix<>&>(matrix);
	double zero = 0;
	Matrix2D<> result(nRows, nRows, &zero);
	for (int i = 0; i < nRows; i++)
		result.data_[i][i] = matr(i, i);

	return result;
}

Matrix2D<> diagInv(const Matrix<> &matrix) {
	size_t nRows = matrix.nRows(),
		nCols = matrix.nCols();
	if (nRows != nCols)
		throw invalid_argument("diagInv() requires a square matrix");

	Matrix<> &matr = const_cast<Matrix<>&>(matrix);
	Matrix2D<> result = matrix;
	for (int i = 0; i < nRows; i++)
		result.data_[i][i] = 1/matr(i, i);

	return result;
}

double mNorm(const Matrix<> &matrix) {
	size_t nRows = matrix.nRows(),
		nCols = matrix.nCols();
	Matrix<> &matr = const_cast<Matrix<>&>(matrix);

	double mnorm = 0;
	for (size_t i = 0; i < nRows; i++) {
		double sumOfAbs = 0;
		for (size_t j = 0; j < nCols; j++)
			sumOfAbs += abs(matr(i, j));
		if (sumOfAbs > mnorm)
			mnorm = sumOfAbs;
	}

	return mnorm;
}

// ***********************
// *** Random matrices ***
// ***********************
Matrix2D<> rndMatrix(size_t nRows, size_t nCols, double start, double end, double precision = 0) {
	Matrix2D<> matr(nRows, nCols);
	for (size_t i = 0; i < nRows; i++)
		for (size_t j = 0; j < nCols; j++)
			matr.data_[i][j] = rnd(start, end, precision);
	return matr;
}

// Matrix having the necessary diagonally dominance
Matrix2D<> rndAWithDiagDom(size_t n, double start, double end, double diagDom, double precision = 0) {
	diagDom = abs(diagDom);
	const size_t normRowInd = 0;
	const double k1 = 1 + 1.0/n, k2 = 100;
	Matrix2D<> matr(n, n);
	double** data = matr.data_;

	for (size_t i = 0; i < n; i++) {
		double upper = rnd(start, end, precision);
		data[i][i] = upper;
		double bound = diagDom*abs(upper);
		double offset = bound/k1;

		for (size_t j = 0; j < n - 1; j++) {
			if (j != i) {
				double x = rnd(bound/(n - j + k2), bound - offset, precision);
				data[i][j] = rndSign()*x;
				bound -= abs(x);
				offset = bound/k1;
			}
		}

		if (i != n - 1)
			data[i][n - 1] = (i == normRowInd) ?
				bound : (rndSign()*rnd(bound/k2, bound - offset, precision));
	}

	return matr;
}
}
