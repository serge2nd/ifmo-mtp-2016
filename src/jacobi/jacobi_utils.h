#pragma once
#include <vector>
#include "../common/utils.h"
#include "../common/matrix_utils.h"
#include "../common/mpi_utils.h"

using std::vector;
using ifmo_mtp_utils::Matrix;
using ifmo_mtp_utils::Matrix2D;
using ifmo_mtp_utils::rndMatrix;
using ifmo_mtp_utils::rndAWithDiagDom;
using ifmo_mtp_utils::splitSize;
using ifmo_mtp_utils::mpiGetCount;
using ifmo_mtp_utils::isend;

namespace ifmo_mtp_jacobi {
double doIteration(Matrix<> &matrSlice, double* x, double* nextX, int totalSize, int sliceStart, int sliceSize) {
	double currEps = 0;
	for (int i = 0; i < sliceSize; i++) {
		int i_ = i + sliceStart;
		nextX[i] = matrSlice(i, totalSize);
		for (int j = 0; j < totalSize; j++)
			if (j != i_)
				nextX[i] -= (matrSlice(i, j)*x[j]);
		nextX[i] /= matrSlice(i, i_);

		double diff = abs(nextX[i] - x[i_]);
		if (diff > currEps)
			currEps = diff;
	}
	return currEps;
}

void generateLSE(int totalSize, double rangeStart, double rangeEnd, double q, Matrix<> &matr, Matrix<> &x) {
	Matrix2D<> a = rndAWithDiagDom(totalSize, rangeStart, rangeEnd, q);
	x = rndMatrix(totalSize, 1, rangeStart, rangeEnd);
	Matrix2D<> b = mp(a, x);

	for (int i = 0; i < totalSize; i++) {
		for (int j = 0; j < totalSize; j++)
			matr(i, j) = a(i, j);
		matr(i, totalSize) = b(i);
	}
}
}