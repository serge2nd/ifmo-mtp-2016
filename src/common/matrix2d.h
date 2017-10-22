#pragma once
#include <iostream>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include "matrix.h"

namespace ifmo_mtp_utils {
using std::istream;
void requireRange(int64_t i, int64_t end, int64_t start = 0);
template<class T> T* newArray(size_t n);
double rnd(double start, double end, double precision = 0);
void unsupported();

template<class T = double> class Matrix2D : public Matrix<T> {
	size_t nRows_, nCols_;
	T** data_;

	void copyFrom(const Matrix<T> &matrix) {
		freeData();
		nRows_ = matrix.nRows();
		nCols_ = matrix.nCols();

		data_ = new T*[nRows_];
		Matrix<T> &matr = const_cast<Matrix<T>&>(matrix);
		for (size_t i = 0; i < nRows_; i++) {
			data_[i] = newArray<T>(nCols_);
			for (size_t j = 0; j < nCols_; j++)
				data_[i][j] = matr(i, j);
		}

		initWrappers();
	}

	void freeData() {
		if (data_ != NULL) {
			for (size_t i = 0; i < nRows_; i++) {
				for (size_t j = 0; j < nCols_; j++)
					data_[i][j].~T();
				operator delete[](data_[i]);
			}
			delete[] data_;
			operator delete[](rows_ - thisSizeInUnits(sizeof(RowWrapper)));
			operator delete[](cols_ - thisSizeInUnits(sizeof(ColWrapper)));
		}
	}

public:
	Matrix<T> &slf = *this;

	/**
	 * Constructor of a matrix.
	 * If initialValuePtr is not presented,
	 * the default constructor of matrix element type is required,
	 * otherwise the copy operation should be overloaded
	 */
	Matrix2D(size_t nRows, size_t nCols = 1, const T* initialValuePtr = NULL)
	{
		data_ = NULL; nRows_ = nCols_ = 0;
		if (nRows > 0 && nCols > 0) {
			nRows_ = nRows; nCols_ = nCols;
			data_ = new T*[nRows];

			if (initialValuePtr == NULL) {
				for (size_t i = 0; i < nRows; i++) {
					data_[i] = new(newArray<T>(nCols)) T[nCols];
				}
			} else {
				for (size_t i = 0; i < nRows; i++) {
					data_[i] = newArray<T>(nCols);
					for (size_t j = 0; j < nCols; j++)
						data_[i][j] = *initialValuePtr;
				}
			}

			initWrappers();
		}
	}
	
	Matrix2D() : Matrix2D(0, 0) {}

	// Copy constructor
	Matrix2D(const Matrix2D<T> &matrix)
		: Matrix2D(dynamic_cast<const Matrix<T>&>(matrix)) {}
	Matrix2D(const Matrix<T> &matrix) {
		data_ = NULL;
		copyFrom(matrix);
	}

	// Input matrix
	template<class U>
	friend void loadMatrix(istream &stream, Matrix<U> &matrix, bool replace, size_t nRows, size_t nCols);

	// Random matrices
	friend Matrix2D<> rndMatrix(size_t nRows, size_t nCols, double start, double end, double precision);
	friend Matrix2D<> rndAWithDiagDom(size_t n, double start, double end, double mnorm, double precision);

	// Derived matrices
	template<class U>
	friend Matrix2D<U> t(const Matrix<U> &matrix);
	friend Matrix2D<> e(size_t n);
	friend Matrix2D<> diag(const Matrix<> &matrix);
	friend Matrix2D<> diagInv(const Matrix<> &matrix);

	// Assignment
	Matrix<T> &operator=(const Matrix<T> &matrix) override {
		copyFrom(matrix);
		return *this;
	}

	size_t nRows() const override { return nRows_; }
	size_t nCols() const override { return nCols_; }

	/**
	 * Access for matrix element
	 */
	T &operator()(size_t i) override {
		requireRange(i, nRows_);
		return data_[i][0];
	}
	T &operator()(size_t i, size_t j) override {
		requireRange(i, nRows_);
		requireRange(j, nCols_);
		return data_[i][j];
	}

	Matrix<T> &row(size_t i) override {
		requireRange(i, nRows_);
		return rows_[i];
	}

	Matrix<T> &col(size_t i) override {
		requireRange(i, nCols_);
		return cols_[i];
	}

	~Matrix2D() override {
		freeData();
	}

private:
	class RowWrapper : public Matrix<T> {
		size_t rowNum_;
		Matrix2D<T> &outer() const {
			return **castMatrix2DPPtr(
					this - rowNum_ - thisSizeInUnits(sizeof(RowWrapper)));
		}
	public:
		RowWrapper(size_t rowNum)
			: rowNum_(rowNum) {}

		size_t nRows() const override { return 1; }
		size_t nCols() const override { return outer().nCols(); }

		Matrix<T> &operator=(const Matrix<T> &matrix) override {
			size_t nCols = this->nCols();
			if (matrix.nRows() != 1 || matrix.nCols() != nCols)
				throw std::invalid_argument("Same-length row is required");

			Matrix<T> &matr = const_cast<Matrix<T>&>(matrix);
			for (size_t i = 0; i < nCols; i++)
				outer()(rowNum_, i) = matr(0, i);

			return *this;
		}

		T &operator()(size_t i) override {
			return outer()(rowNum_, i);
		}
		T &operator()(size_t i, size_t j) override {
			requireRange(i, 1);
			return outer()(rowNum_, j);
		}

		Matrix<T> &row(size_t i) override {
			requireRange(i, 1);
			return *this;
		}

		Matrix<T> &col(size_t i) override { unsupported(); return *this; }
	};

	class ColWrapper : public Matrix<T> {
		size_t colNum_;
		Matrix2D<T> &outer() const {
			return **castMatrix2DPPtr(
					this - colNum_ - thisSizeInUnits(sizeof(ColWrapper)));
		}
	public:
		ColWrapper(size_t colNum)
			: colNum_(colNum) {}

		size_t nRows() const override { return outer().nRows(); }
		size_t nCols() const override { return 1; }

		Matrix<T> &operator=(const Matrix<T> &matrix) override {
			size_t nRows = this->nRows();
			if (matrix.nCols() != 1 || matrix.nRows() != nRows)
				throw std::invalid_argument("Same-length column is required");

			Matrix<T> &matr = const_cast<Matrix<T>&>(matrix);
			for (size_t i = 0; i < nRows; i++)
				outer()(i, colNum_) = matr(i, 0);

			return *this;
		}

		T &operator()(size_t i) override {
			return outer()(i, colNum_);
		}
		T &operator()(size_t i, size_t j) override {
			requireRange(j, 1);
			return outer()(i, colNum_);
		}

		Matrix<T> &col(size_t i) override {
			requireRange(i, 1);
			return *this;
		}

		Matrix<T> &row(size_t i) override { unsupported(); return *this; }
	};

private:
	RowWrapper* rows_;
	ColWrapper* cols_;

	void initWrappers() {
		size_t thisPtrNUnits = thisSizeInUnits(sizeof(RowWrapper));
		rows_ = newArray<RowWrapper>(nRows_ + thisPtrNUnits);

		*castMatrix2DPPtr(rows_) = this;
		rows_ += thisPtrNUnits;
		for (size_t i = 0; i < nRows_; i++)
			new(rows_ + i) RowWrapper(i);

		thisPtrNUnits = thisSizeInUnits(sizeof(ColWrapper));
		cols_ = newArray<ColWrapper>(nCols_ + thisPtrNUnits);

		*castMatrix2DPPtr(cols_) = this;
		cols_ += thisPtrNUnits;
		for (size_t i = 0; i < nCols_; i++)
			new(cols_ + i) ColWrapper(i);
	}

	static size_t thisSizeInUnits(size_t unitSize) {
		return sizeof(Matrix2D<T>*)/unitSize + 1;
	}

	static Matrix2D<T>** castMatrix2DPPtr(const void* ptr) {
		return static_cast<Matrix2D<T>**>(
				const_cast<void*>(ptr));
	}
};
}
